long long make_change(vector<int> &coins, int money) 
{
    std::vector<long long> res(money+1);
    
    res[0] = 1;
    
    for(const auto &coin : coins)
    {
        for(auto idx = coin; idx < res.size(); ++idx)
            res[idx] += res[idx-coin];
    }    
    
    
    return res[money];
}