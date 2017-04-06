#include <iostream>

template<unsigned n>
struct Get 
{
 template<typename X, typename ...Xs>
 constexpr auto operator()(X x, Xs ...xs) 
 {
   if constexpr(n > sizeof...(xs)) 
   {
     return;
   } 
   else if constexpr(n > 0) 
   {
     return Get<n-1>{}(xs...);
   } 
   else 
   {
     return x;
   }
 }
};
template<unsigned n>
auto arg = Get<n>{};

int main()
{
	std::cout<<"ARG: "<<arg<2>(1,2,"I GOT YOU",4,5,6,7);

	return 0;
}