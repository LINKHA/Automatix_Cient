// This file is part of CAF, the C++ Actor Framework. See the file LICENSE in
// the main distribution directory for license terms and copyright or visit
// https://github.com/actor-framework/actor-framework/blob/master/LICENSE.

#pragma once

#include "caf/detail/atomic_ref_counted.hpp"
#include "caf/detail/scope_guard.hpp"
#include "caf/flow/observer.hpp"
#include "caf/flow/op/hot.hpp"
#include "caf/flow/subscription.hpp"
#include "caf/sec.hpp"

#include <utility>

namespace caf::flow::op {

/// Reads from an observable buffer and emits the consumed items.
template <class Buffer>
class from_resource_sub : public detail::atomic_ref_counted,
                          public subscription::impl,
                          public async::consumer {
public:
  // -- member types -----------------------------------------------------------

  using value_type = typename Buffer::value_type;

  using buffer_ptr = intrusive_ptr<Buffer>;

  // -- constructors, destructors, and assignment operators --------------------

  from_resource_sub(coordinator* parent, buffer_ptr buf,
                    observer<value_type> out)
    : parent_(parent), buf_(buf), out_(std::move(out)) {
    parent_->ref_execution_context();
  }

  ~from_resource_sub() {
    parent_->deref_execution_context();
  }

  // -- implementation of subscription_impl ------------------------------------

  coordinator* parent() const noexcept override {
    return parent_.get();
  }

  bool disposed() const noexcept override {
    return disposed_;
  }

  void dispose() override {
    CAF_LOG_TRACE("");
    if (!disposed_) {
      disposed_ = true;
      if (!running_)
        do_dispose();
    }
  }

  void request(size_t n) override {
    CAF_LOG_TRACE(CAF_ARG(n));
    if (demand_ != 0) {
      demand_ += n;
    } else {
      demand_ = n;
      run_later();
    }
  }

  // -- implementation of async::consumer: these may get called concurrently ---

  void on_producer_ready() override {
    // nop
  }

  void on_producer_wakeup() override {
    CAF_LOG_TRACE("");
    parent_->schedule_fn([ptr = strong_this()] {
      CAF_LOG_TRACE("");
      ptr->running_ = true;
      ptr->do_run();
    });
  }

  // -- intrusive_ptr interface ------------------------------------------------

  friend void intrusive_ptr_add_ref(const from_resource_sub* ptr) noexcept {
    ptr->ref();
  }

  friend void intrusive_ptr_release(const from_resource_sub* ptr) noexcept {
    ptr->deref();
  }

  void ref_consumer() const noexcept override {
    this->ref();
  }

  void deref_consumer() const noexcept override {
    this->deref();
  }

  void ref_disposable() const noexcept override {
    this->ref();
  }

  void deref_disposable() const noexcept override {
    this->deref();
  }

  void ref_coordinated() const noexcept override {
    this->ref();
  }

  void deref_coordinated() const noexcept override {
    this->deref();
  }

private:
  void run_later() {
    if (!running_) {
      running_ = true;
      parent_->delay_fn([ptr = strong_this()] { ptr->do_run(); });
    }
  }

  void do_dispose() {
    if (buf_) {
      auto tmp = std::move(buf_);
      tmp->cancel();
    }
    if (out_)
      out_.on_complete();
  }

  void do_run() {
    CAF_LOG_TRACE("");
    auto guard = detail::scope_guard([this]() noexcept { running_ = false; });
    if (disposed_) {
      do_dispose();
      return;
    }
    CAF_ASSERT(out_);
    CAF_ASSERT(buf_);
    while (demand_ > 0) {
      auto [again, pulled] = buf_->pull(async::delay_errors, demand_, out_);
      if (!again) {
        buf_ = nullptr;
        out_.release_later();
        disposed_ = true;
        return;
      } else if (disposed_) {
        do_dispose();
        return;
      } else if (pulled == 0) {
        return;
      } else {
        CAF_ASSERT(demand_ >= pulled);
        demand_ -= pulled;
      }
    }
  }

  intrusive_ptr<from_resource_sub> strong_this() {
    return {this};
  }

  /// Stores the @ref coordinator that runs this flow. Unlike other observables,
  /// we need a strong reference to the coordinator because otherwise the buffer
  /// might call `schedule_fn` on a destroyed object.
  intrusive_ptr<coordinator> parent_;

  /// Stores a pointer to the asynchronous input buffer.
  buffer_ptr buf_;

  /// Stores a pointer to the target observer running on `remote_parent_`.
  observer<value_type> out_;

  /// Stores whether do_run is currently running.
  bool running_ = false;

  /// Stores whether dispose() has been called.
  bool disposed_ = false;

  /// Stores the demand from the observer.
  size_t demand_ = 0;
};

template <class T>
class from_resource : public hot<T> {
public:
  // -- member types -----------------------------------------------------------

  using resource_type = async::consumer_resource<T>;

  using super = hot<T>;

  // -- constructors, destructors, and assignment operators --------------------

  from_resource(coordinator* parent, resource_type resource)
    : super(parent), resource_(std::move(resource)) {
    // nop
  }

  // -- implementation of observable_impl<T> -----------------------------------

  disposable subscribe(observer<T> out) override {
    CAF_LOG_TRACE("");
    CAF_ASSERT(out);
    if (resource_) {
      if (auto buf = resource_.try_open()) {
        resource_ = nullptr;
        using buffer_type = typename resource_type::buffer_type;
        CAF_LOG_DEBUG("add subscriber");
        using impl_t = from_resource_sub<buffer_type>;
        auto ptr = super::parent_->add_child(std::in_place_type<impl_t>, buf,
                                             out);
        buf->set_consumer(ptr);
        super::parent_->watch(ptr->as_disposable());
        out.on_subscribe(subscription{ptr});
        return ptr->as_disposable();
      }
      resource_ = nullptr;
      CAF_LOG_WARNING("failed to open an async resource");
      return super::fail_subscription(
        out, make_error(sec::cannot_open_resource,
                        "failed to open an async resource"));
    }
    CAF_LOG_WARNING("may only subscribe once to an async resource");
    return super::fail_subscription(
      out, make_error(sec::too_many_observers,
                      "may only subscribe once to an async resource"));
  }

private:
  resource_type resource_;
};

} // namespace caf::flow::op
