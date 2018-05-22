int binarySearch(int first, int last, vector<IceCream> &arr, int search) 
{
   if(first > last)
       return -1;
   
    int mid = first + ((last - first)/2);
    
    if(arr[mid] == search)
        return mid;
    
    if(search > arr[mid])
    {
        return binarySearch(mid+1, last, arr, search);
    }
    else
    {
        return binarySearch(first, mid-1, arr, search);
    }
    
}