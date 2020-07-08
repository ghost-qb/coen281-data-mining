
/* main.cpp
coen281-program-1-QB

Created by Quan Bach  on 6/29/20.
Copyright © 2020 QB. All rights reserved.

 */

#include "P1.hpp"


int main()
{
    /* set up vairables */
    std::ifstream input_list_of_file;
    std::vector<std::string> file_name_vector = {};
    std::string input_file_name = "";
    std::string file_name = "";
    std::vector<std::string> all_shingles_vector;
    std::vector<std::vector<std::string>> shingles_by_document;
    /* Number of Permutation */
    int permunation_input_num = 30;
    /* r_value; b_value; and s_value */
    int b = 6;
    int r = permunation_input_num/b;
    double s = 0.8;

    /*get user input for file name*/
    std::cout << "Please enter input file name: ";
    std::cin >> input_file_name;
    
    /* read in input files to a vector */
    try {
        input_list_of_file.open(input_file_name, std::ios::in);
        while (input_list_of_file >> file_name)
        {
            
            file_name_vector.push_back(file_name);
        }
        if (file_name_vector.size() == 0)
        {
            std::cout << "ERROR: File entered is empty. Please only enter file that is not empty." << std::endl;
            return -1;
        }
        input_list_of_file.close();
        
    }
    catch(std::exception const &e) {
        std::cerr << "Error Occured: " << e.what() << std::endl;
    }
    
    /* ******************************** READING IN INPUT FILE AND EXCEPTION HANDLING ********************************************* */
    /* READING ENTIRE FILE INTO A STRING & REMOVE WHITE SPACES & CONVERT TO LOWERCASE*/
    try {
        for (int i = 0; i < file_name_vector.size();i++)
        {
            std::ifstream input_file_to_string;
            try {
                input_file_to_string.open(file_name_vector[i]);
            }
            catch(std::exception const &e){
                std::cerr << "Error Occured: " << e.what() << std::endl;
            }
            
            std::string file_contents ((std::istreambuf_iterator<char>(input_file_to_string)),std::istreambuf_iterator<char>());
            
            if (file_contents == "")
            {
                std::cout << "ERROR: One of the file is empty. Please only use file with content." << std::endl;
                return -1;
            }
            
            int j = 0;
            while(j < file_contents.length())       /* remove consecutive spaces to become single space */
            {
                if (::isspace(file_contents.at(j)))
                {
                    int k = j + 1;
                    while (k < file_contents.length() && ::isspace(file_contents.at(k)))
                    {
                        file_contents.erase(k,1);
                    }
                }
                j++;
            }
            
            std::replace_if(file_contents.begin(), file_contents.end(), ::isspace, ' '); /* replace all white spaces with space */
            file_contents.erase(std::remove_if(file_contents.begin(),file_contents.end(),::ispunct),file_contents.end()); /* remove punnctuations */
            std::for_each(file_contents.begin(),file_contents.end(),[] (char & c) { c = ::tolower(c);}); /* convert to lowercase */
            
            std::vector<std::string> shingles_vector = generate_shingles_vector(file_contents);
            for (int i = 0; i < shingles_vector.size();i++)
            {
                all_shingles_vector.push_back(shingles_vector[i]);
            }
            shingles_by_document.push_back(shingles_vector);
            
            input_file_to_string.close();
            
        }
    }
    catch(std::exception const &e) {
        std::cerr << "Error Occured: " << e.what() << std::endl;
    }
    
    
    
    
    /* ******************************* SHINGLING **************************************************************/
    /* Make All Shingles Vector To Set i.e. contains only unique elements */
    std::unordered_set<std::string> all_shingles_set (all_shingles_vector.begin(),all_shingles_vector.end());
    all_shingles_vector.assign(all_shingles_set.begin(), all_shingles_set.end());
   
    
    /* Hash All Shingles Vector */
    
    std::vector<int> hashed_all_shingles_vector;
    for (int i =0; i < all_shingles_vector.size(); i++)
    {
        int hashed_value = hash_shingle(all_shingles_vector[i]);
        if (std::find(hashed_all_shingles_vector.begin(),hashed_all_shingles_vector.end(),hashed_value) != hashed_all_shingles_vector.end())
        {
            continue;
        }
        else
        {
            hashed_all_shingles_vector.push_back(hashed_value);
        }
        
    }
    
    /* Hash Shingles By Documents */
    std::vector<std::vector<int>> hashed_shingles_by_document;
    for (int i = 0; i < shingles_by_document.size(); i++)
    {
        std::vector<int> hashed_to_int;
        for (int j =0; j < shingles_by_document[i].size(); j++)
        {
            hashed_to_int.push_back(hash_shingle(shingles_by_document[i][j]));
            std::unordered_set<int> temp_set (hashed_to_int.begin(),hashed_to_int.end());
            hashed_to_int.assign(temp_set.begin(), temp_set.end());
        }
        hashed_shingles_by_document.push_back(hashed_to_int);
    }


    /* ****************************** MinHashing ****************************************************************** */
    /* Create The Characteristic Matrix */

    std::vector<std::vector<int>> characteristic_matrix;
    
    for(int i = 0; i < hashed_shingles_by_document.size(); i++)
    {
        std::vector<int> int_vector;
        for (int j = 0; j < hashed_all_shingles_vector.size(); j++)
        {
            if (std::find(hashed_shingles_by_document[i].begin(), hashed_shingles_by_document[i].end(), hashed_all_shingles_vector[j]) != hashed_shingles_by_document[i].end())
            {
                int_vector.push_back(1);
            }
            else {
                int_vector.push_back(0);
            }
        }
        characteristic_matrix.push_back(int_vector);
    }
    
    
    /* Construct the permutation vector */
    
    std::vector<std::vector<int>> permutation_list;
    
    std::vector<int> prime_list;
    prime_list.push_back(1);
    int scalar = 3;
    
    for (int i = 1; i <= permunation_input_num; i++) /* Create a list of prime number to compute the permutations */
    {
        if (isPrime(scalar))
        {
            prime_list.push_back(scalar);
            scalar++;
        }
        else{
            i--;
            scalar++;
        }
    }
    
    
    
    std::vector<int> permutation;
    for (int i = 0; i < hashed_all_shingles_vector.size(); i++) /* push back the first permutation 0-N */
    {
        permutation.push_back(i);
    }
    
    permutation_list.push_back(permutation);
    
    for (int i = 0; i < permunation_input_num - 1; i++)
    {
        std::vector<int> permutation;
        for (int j = 0; j < hashed_all_shingles_vector.size(); j++)
        {
            permutation.push_back(((j * prime_list[i])+1) % hashed_all_shingles_vector.size());
        }
        permutation_list.push_back(permutation);
    }
    
    
    /* Print out the Permutation | Characteristic Matrix table */
    
    std::cout << "Permutations | Charateristic Matrix" << std::endl;
    for (int i = 0; i < hashed_all_shingles_vector.size(); i++)
    {
        for (int k = 0; k < permutation_list.size(); k++)
        {
            std::cout << std::left << std::setw(4) << permutation_list[k][i] << " ";
        }
        
        std::cout << std::right << std::setw(int(characteristic_matrix.size()*2 + 5));
        for (int j = 0; j < characteristic_matrix.size(); j++)
        {
            std::cout << characteristic_matrix[j][i] << " ";
        }
        std::cout << std::endl;
    }

    /* Free up memory from shingle vectors */
    std::vector<std::string>().swap(all_shingles_vector);
    std::vector<std::vector<std::string>>().swap(shingles_by_document);
    
    /* ******************************** SIGNATURE MATRIX *********************************************************** */
    std::cout << std::endl << std::endl;
    /* Print out the Signatures Matrix */
    
    std::vector<std::vector<int>> signature_matrix_transpose;
    
    std::cout << "Signature Matrix" << std::endl;
    for (int i = 0; i < permutation_list.size(); i++)
    {
        std::vector<int> entry_to_sig_matrix;
        for (int j = 0; j < characteristic_matrix.size(); j++)
        {
            int k = 0;
            bool done = false;
            while (!done)
            {
                auto it = std::find(permutation_list[i].begin(), permutation_list[i].end(), k);
                int pos = int(std::distance(permutation_list[i].begin(), it));
                if (characteristic_matrix[j][pos] == 1)
                {
                    std::cout << std::left << std::setw(5) << permutation_list[i][pos] << "\t";
                    entry_to_sig_matrix.push_back(permutation_list[i][pos]);
                    done = true;
                }
                
                if (k > 5000)
                {
                    entry_to_sig_matrix.push_back(-1);
                    done = true;
                }
                k++;
                
            }
        }
        signature_matrix_transpose.push_back(entry_to_sig_matrix);
        std::cout << std::endl;
    }
    
    /* Convert the Transpose Sig Matrix */
    
    std::vector<std::vector<int>> signature_matrix;
    for (int i = 0; i < signature_matrix_transpose[i].size();i++)
    {
        std::vector<int> entry;
        for (int j =0; j < signature_matrix_transpose.size(); j++)
        {
            entry.push_back(signature_matrix_transpose[j][i]);
        }
        signature_matrix.push_back(entry);
    }
    
    
    /* Compute ()-sensitive family */
    std::cout << std::endl;
    std::cout << "()-sensitive family:" << std::endl;
    double p = pow(1-pow(1-s,b),r);
    std::cout << "(" << double(1-s) << "," << s << "," << std::fixed << std::setprecision(4) << p << "," << 1-p << ")" << std::endl;
    std::cout << std::endl;
    
    std::cout << "(b,r): (" << b << ", " << r << ") : OR-AND" << std::endl;
    
    
    /* **************************** LSH ************************************** */

    std::unordered_multimap<int,int> LSH_map;
    std::vector<int> key_set;
    std::set<std::set<int>> candidate_pairs_by_band;
    
    for (int i = 0; i < b; i++)
    {
        for (int j = 0; j < signature_matrix.size(); j++)
        {
            std::vector<int> tmp;
            std::copy((signature_matrix[j].begin() + i*(b-1)), (signature_matrix[j].begin() + i*(b-1)+(b-1)), std::back_inserter(tmp));
            int result = LSH_hash_function(tmp);
            LSH_map.insert({result,j});
            key_set.push_back(result);
        }
    }
    
    
    /* Construct Candidate Pairs by Band from LSH Map */
    for (int i = 0; i < key_set.size(); i++)
    {
        std::set<int> tmp;
        for (auto itr = LSH_map.begin(); itr != LSH_map.end(); itr++)
        {
            if (itr->first == key_set[i])
            {
                tmp.insert(itr->second);
            }
        }
        candidate_pairs_by_band.insert(tmp);
    }
    
    /* Construct the Candidate Pairs */
    
    std::set<std::vector<int>> all_candidate_pairs;
    
    for (int i = 0; i < candidate_pairs_by_band.size(); i++)
    {
        auto setIt = candidate_pairs_by_band.begin();
        for (int j = 0; j < i; j++)
        {
            setIt++;
        }
        
        auto tmp_set = *setIt;
        if (tmp_set.size() > 1)
        {
            int traverse_size = int(tmp_set.size()*(tmp_set.size()-1)/2);
            for (int i = 0; i < traverse_size; i++)
            {
                std::vector<int> candidate_pair;
                for (int q = 0; q < tmp_set.size();q++)
                {
                    for (int r = q + 1; r < tmp_set.size(); r++)
                    {
                        auto mIt = tmp_set.begin();
                        for (int m = 0; m < q;m++)
                        {
                            mIt++;
                        }
                        candidate_pair.push_back(*mIt);
                        
                        auto qIt = tmp_set.begin();
                        for (int n = 0; n < r; n++)
                        {
                            qIt++;
                        }
                        candidate_pair.push_back(*qIt);
                    }
                    all_candidate_pairs.insert(candidate_pair);
                }
                
            }
        }
    }
    
    
    /* Compare the Candidate Pairs and Print Out the Pair with More Than 80% Similarity */
    std::cout << std::endl << std::endl;
    
    if (all_candidate_pairs.size() == 0)
    {
        std::cout << "No pair of files has similarity more than 80%!" << std::endl;
        return 0;
    }
    
    std::cout << "Files that have more than 80% similarity: " << std::endl;
    for (int i = 0; i < all_candidate_pairs.size(); i++)
    {
        auto it = all_candidate_pairs.begin();
        for (int j = 0; j < i; j++)
        {
            it++;
        }
        std::vector<int> it_vec = *it;
        std::vector<int> vector_difference;
        std::vector<int> v1 = signature_matrix[it_vec[0]];
        std::vector<int> v2 = signature_matrix[it_vec[1]];
        std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
        std::inserter(vector_difference, vector_difference.begin()));
        double percentage_difference = double(vector_difference.size())/double(v1.size());
        
        if (percentage_difference <= 0.2)
        {
            std::cout << file_name_vector[it_vec[0]] << " and " << file_name_vector[it_vec[1]] << std::endl;
        }
        else
        {
            std::cout << "Pair " << file_name_vector[it_vec[0]] << " and " << file_name_vector[it_vec[1]] << " is a candidate pair BUT is found to be NOT similar." << std::endl;
        }
        
    }
    
    std::cout << std::endl;
    
    
    return 0;
}



/* function hash_shingle will turn a string into an int value
    funtion will sum the ascii values of the string and mod with a prime number and returns a hashed int value for the input string
 */
int hash_shingle (std::string str)
{
    int hashed_shingle_value = 0;
    int total_ascii_sum = 0;
    
    for (int i = 0; i < str.length(); i++)
    {
        char c = str.at(i);
        total_ascii_sum += (int(c) % 9) * pow(10, i);
    }
        
    hashed_shingle_value = total_ascii_sum;
    
    return hashed_shingle_value;
}



/* function generate_shingles_vector
    this function takes a string as input and perform shingling with k_factor abd store the output to a vector. Then return that vector.
 */
std::vector<std::string> generate_shingles_vector (std::string &str)
{
    std::vector<std::string> shingles_vector = {};
    
    int start_index = 0;
    int k_factor = 9;
    if (str.length() >= 9)
    {
        while (start_index != str.length() - 8)
        {
            shingles_vector.push_back(str.substr(start_index,k_factor));
            start_index++;
        }
    }
    
    return shingles_vector;
}

/* Function IsPrime determines if an input is a prime number */
bool isPrime (int n)
{
    if (n <= 1)
    {
        return false;
    }
    
    for (int i = 2; i < n; i++)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}

int LSH_hash_function (std::vector<int> vec)
{
    int result = 1;
    for (int i = 0; i < vec.size(); i++)
    {
        if(vec[i] != 0)
        {
            result *= vec[i]*17;
        }
    }
    
    return result % 2000093;
}
