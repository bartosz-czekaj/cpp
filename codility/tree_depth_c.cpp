int solution(struct tree * T) 
{
   int ldepth = 0;
   int rdepth = 0;
   if(T == 0)
      return -1;
   ldepth=solution(T->l);
   rdepth=solution(T->r);
   if(ldepth>rdepth)
      return (ldepth+1);
   else
     return (rdepth+1);
}