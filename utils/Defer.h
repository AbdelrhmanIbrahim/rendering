namespace utils
{
	template <typename F>
	struct Defer {
		F functor;
		Defer(F functor) : functor(functor) {}
		~Defer() { functor(); }
	};
};

#define defer(name, code) utils::Defer name([&](){code;});