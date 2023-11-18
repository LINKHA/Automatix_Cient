#pragma once

#include <memory>
#include <mutex>

namespace amx {

#define REGISTER_SINGLETON(_This) \
public:\
	friend struct TSingleton<_This>;\
	static _This& Get();\

#define IMPLEMENT_SINGLETON(_This) \
	_This& _This::Get(){ return TSingleton<_This>::Instance(); }

	template <typename _Ty>
	struct TSingleton
	{
	private:
		struct ObjectCreator
		{
			ObjectCreator() { TSingleton<_Ty>::Instance(); }
			inline void DoNothing() const { }
		};
		static ObjectCreator createObject;

		static std::mutex mutex;  // Add a mutex for thread safety

		TSingleton();

	public:
		typedef _Ty ObjectType;

		static ObjectType& Instance()
		{
			static ObjectType obj;
			std::lock_guard<std::mutex> lock(mutex);  // Use lock_guard to lock and unlock the mutex automatically
			createObject.DoNothing();

			return obj;
		}
	};

	template <typename _Ty>
	typename TSingleton<_Ty>::ObjectCreator
		TSingleton<_Ty>::createObject;

	template <typename _Ty>
	std::mutex TSingleton<_Ty>::mutex;  // Initialize the static mutex

}