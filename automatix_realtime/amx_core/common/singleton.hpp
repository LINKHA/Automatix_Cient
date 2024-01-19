#pragma once

#include <memory>
#include <mutex>

namespace amx {

#define REGISTER_SINGLETON(_This) \
public:\
	friend struct singleton<_This>;\
	static _This& get();\

#define IMPLEMENT_SINGLETON(_This) \
	_This& _This::get(){ return singleton<_This>::instance(); }

	template <typename _Ty>
	struct singleton
	{
	private:
		struct object_creator
		{
			object_creator() { singleton<_Ty>::instance(); }
			inline void do_nothing() const { }
		};
		static object_creator createObject;

		static std::mutex mutex;  // Add a mutex for thread safety

		singleton();

	public:
		typedef _Ty object_type;

		static object_type& instance()
		{
			static object_type obj;
			std::lock_guard<std::mutex> lock(mutex);  // Use lock_guard to lock and unlock the mutex automatically
			createObject.do_nothing();

			return obj;
		}
	};

	template <typename _Ty>
	typename singleton<_Ty>::object_creator
		singleton<_Ty>::createObject;

	template <typename _Ty>
	std::mutex singleton<_Ty>::mutex;  // Initialize the static mutex

}