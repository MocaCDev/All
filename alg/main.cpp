// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>

bool has_triangle(char size, std::vector<char> table)
{
    int pos = size; /* skip first row. */
    
    if(table[pos] != 0) return false;
    pos += size;
    
    if(size == 3)
    {
        if(table[pos] == 0 && table[pos + 1] == 0) return true;
        
        return false;
    }
        
    
    /* Assume >= 4x4. */
    for(int i = 0; i < size - 2; i++)
    {
        for(int x = 0; x < i + 2; x++)
            if(table[pos + x] != 0) return false;
            
        pos += size;
    }
    
    return true;
}

int main() {
    std::vector<char> table{
        1,2,3,
        0,2,3,
        0,1,3
    };
    
    if(has_triangle(3, table))
        std::cout << "Yes" << std::endl;
    else
        std::cout << "No" << std::endl;

    return 0;
}
