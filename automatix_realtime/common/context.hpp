#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <any>
#include <unordered_map>
#include <shared_mutex>
#include <map>
#include <memory>

namespace amx {

class context : public std::enable_shared_from_this<context> {
public:
	// Constructor, initializes the Context 
	// (���캯������ʼ��Context)
	context() :
		_canceled(false),
		_deadline(std::chrono::steady_clock::time_point::max())
	{}

	// WithDeadline sets the timeout time 
	// (WithDeadline �������ó�ʱʱ��)
	std::shared_ptr<context> with_deadline(std::chrono::steady_clock::time_point deadline) {
		_deadline = deadline;
		return shared_from_this();
	}

	// WithCancel sets the cancellation flag and specifies a function to be executed on cancellation
	// (WithCancel ��������ȡ����־����ָ����ȡ��ʱִ�еĺ���)
	std::shared_ptr<context> with_cancel(std::function<void()> cancel_func) {
		_cancel_func = [shared_this = shared_from_this(), cancel_func]() {
			shared_this->cancel();
			if (cancel_func) {
				cancel_func();
			}
			};
		return shared_from_this();
	}

	// Done returns a waitable condition variable to wait for the Context's cancellation signal or timeout 
	// (Done ����һ���ɵȴ����������������ڵȴ�Context��ȡ���źŻ�ﵽ��ʱʱ��)
	std::unique_lock<std::mutex> done() {
		std::unique_lock<std::mutex> lock(_mutex);
		if (_cancel_condition.wait_until(lock, _deadline, [this]() { return _canceled; })) {
			// Exits wait because _canceled became true (�˳��ȴ�����Ϊ _canceled ��Ϊ true)
			return lock;
		}
		else {
			// Exits wait because the _deadline was reached (�˳��ȴ�����Ϊ�ﵽ�� _deadline)
			_canceled = true; // You can choose to set _canceled to true on timeout (����ѡ���ڳ�ʱʱ���� _canceled Ϊ true)
			return lock;
		}
	}

	// Value returns the value in the Context 
	// (Value ����Context�е�ֵ)
	template <typename _Ty>
	_Ty value(const std::string& key) {
		std::shared_lock<std::shared_mutex> lock(_valueMutex);
		auto it = _values.find(key);
		if (it != _values.end()) {
			return std::any_cast<_Ty>(it->second);
		}
		// If the corresponding value is not found, return a default value or throw an exception as needed (����Ҳ�����Ӧֵ�����Ը���ʵ�����󷵻�Ĭ��ֵ���׳��쳣�ȴ���)
		throw std::out_of_range("Key not found");
	}

	// WithValue stores a value in the Context
	//  (WithValue ������Context�д洢ֵ)
	template <typename _Ty>
	std::shared_ptr<context> with_value(const std::string& key, const _Ty& value) {
		std::unique_lock<std::shared_mutex> lock(_valueMutex);
		_values[key] = value;
		return shared_from_this();
	}

	// cancel is used to cancel the Context
	//  (cancel ����ȡ��Context)
	void cancel() {
		std::unique_lock<std::mutex> lock(_mutex);
		_canceled = true;
		_cancel_condition.notify_all();
		if (_cancel_func) {
			_cancel_func();
		}
	}

private:
	mutable std::mutex _mutex;  // Used to protect _canceled and _cancelCondition (���ڱ��� _canceled �� _cancelCondition)
	mutable std::condition_variable _cancel_condition;  // Used to wait for the cancellation signal (���ڵȴ�ȡ���ź�)
	mutable std::shared_mutex _valueMutex;  // Used to protect _values (���ڱ��� _values)
	bool _canceled;  // Flag indicating if Context is canceled (��־Context�Ƿ���ȡ��)
	std::chrono::steady_clock::time_point _deadline;  // Timeout time (��ʱʱ��)
	std::function<void()> _cancel_func;  // Function executed on cancellation (ȡ��ʱִ�еĺ���)
	std::unordered_map<std::string, std::any> _values;  // Stores the values of the Context (�洢Context��ֵ)
};



// ContextManager class for managing multiple contexts
// ContextManager �࣬���ڹ����� context
class ContextManager {
public:
	// Create and store a new context
	// �������洢һ���µ� context
	std::shared_ptr<context> create_context() {
		auto ctx = std::make_shared<context>();
		std::unique_lock<std::mutex> lock(_mutex);
		_contexts[_next_id++] = ctx;
		return ctx;
	}

	// Retrieve a context by its ID
	// ͨ��ID���� context
	std::shared_ptr<context> get_context(int id) {
		std::shared_lock<std::mutex> lock(_mutex);
		auto it = _contexts.find(id);
		if (it != _contexts.end()) {
			return it->second;
		}
		return nullptr; // Return nullptr if not found
	}

	// Cancel a specific context by its ID
	// ͨ��IDȡ���ض��� context
	void cancel_context(int id) {
		std::unique_lock<std::mutex> lock(_mutex);
		auto it = _contexts.find(id);
		if (it != _contexts.end()) {
			it->second->cancel();
		}
	}

	// Cancel all contexts
	// ȡ������ context
	void cancel_all() {
		std::unique_lock<std::mutex> lock(_mutex);
		for (auto& pair : _contexts) {
			pair.second->cancel();
		}
	}

private:
	std::mutex _mutex; // Mutex for thread-safe access
	std::map<int, std::shared_ptr<context>> _contexts; // Map to store contexts with unique IDs
	int _next_id = 0; // ID for the next context
};

} // namespace amx

//// example
//using namespace amx;
//
//int main() {
//	auto baseCtx = std::make_shared<context>();
//	auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
//	auto deadlineCtx = baseCtx->with_deadline(deadline);
//	// Create a cancelable context with a custom cancel function
//	  // ����һ����ȡ����context����ָ���Զ���ȡ������
//	auto cancelableCtx = baseCtx->with_cancel([]() {
//		std::cout << "Context canceled!" << std::endl;
//		});
//
//	auto valueCtx = baseCtx->with_value<std::string>("key", "value");
//	std::string str = baseCtx->value<std::string>("key");
//
//	std::unique_lock<std::mutex> lock = baseCtx->done();
//	// 
//
//	return 0;
//}

//int main() {
//	ContextManager manager;
//
//	// Create a new context
//	auto ctx1 = manager.create_context();
//
//	// Create another context and set a deadline
//	auto ctx2 = manager.create_context();
//	ctx2->with_deadline(std::chrono::steady_clock::now() + std::chrono::seconds(5));
//
//	// Cancel a specific context
//	manager.cancel_context(0);
//
//	// Wait for a context to be done
//	std::unique_lock<std::mutex> lock = ctx2->done();
//
//	return 0;
//}