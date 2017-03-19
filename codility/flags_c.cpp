int solution(int A[], int N) {
    int i,j,k;
    int count=0;
    int countval=0;
    int count1=0;
    int flag;
    for(i=1;i<N-1;i++)
    {
        if((A[i-1]<A[i]) && (A[i]>A[i+1]))
        {



            printf("%d %d\n",A[i],i);
             A[count++]=i;
            i++;

        }
    }  

    j=A[0];
    k=0;
    if (count==1 || count==0)
    return count;
    if (count==2)
    {
        if((A[1]-A[0])>=count)
        return 2;
        else 
        return 1;
    }


    flag=0;
    // contval=count;
    count1=1;
    countval=count;
    while(1)
    {
    for(i=1;i<count;i++)
    {
           printf("%d %d\n",A[i],j);
           if((A[i]-j)>=countval)
            {
                printf("Added %d %d\n",A[i],j);
                count1++;
                j=A[i];

            }
           /* if(i==count-1 && count1<count)
            {
                j=A[0];
                i=0;
                count1=1;
            }*/

    }
printf("count %d count1 %d \n",countval,count1);
if (count1<countval)
            {
               count1=1;
               countval--;
               j=A[0];
              }
            else
            {
              break;            }


}
    return countval;
   }