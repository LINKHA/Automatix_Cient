#pragma once

namespace amx {
	namespace test {
		class automatix_test {
		public:
			explicit automatix_test() {}
			virtual ~automatix_test() {}
			void init();
			void tick();
		};
	}
}