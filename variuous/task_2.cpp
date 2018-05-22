using namespace std;


int  trailingZeros(unsigned long long nb)
{
  int b = 0;
  for(b=0; b < 64 &&  !(nb&1); b++)
  {    
      nb>>=1;
  }
  return b;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int games = 0;
    std::cin >> games;
    
    for (int i = 0; i < games; ++i)
    {
        unsigned long long nb = 0;
        std::cin >> nb;
        
        int count = trailingZeros(nb);
        nb = nb >> count;
        
        while(nb)
        {
            if(nb & 1)
            {
                ++count;
            }     
            nb = nb >> 1;
        } 
        
        std::cout<<((count - 1) % 2 ? "Louise\n" : "Richard\n");
    }    
    
    return 0;
}