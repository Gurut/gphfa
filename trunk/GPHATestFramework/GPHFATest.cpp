/*
 **************************************************************************
 *                                                                        *
 *       General Purpose Hash Function Algorithms Test Framework          *
 *                                                                        *
 * Author: Arash Partow - 2002                                            *
 * URL: http://www.partow.net                                             *
 * URL: http://www.partow.net/programming/hashfunctions/index.html        *
 *                                                                        *
 * Copyright notice:                                                      *
 * Free use of the General Purpose Hash Function Algorithms Library is    *
 * permitted under the guidelines and in accordance with the most current *
 * version of the Common Public License.                                  *
 * http://www.opensource.org/licenses/cpl.php                             *
 *                                                                        *
 **************************************************************************
*/


/*
   Definitions:

   LCL:     Longest chain length
   LCL_CNT: Count of buckets with longest chain length
   NZL:     Number of zero length chaining buckets (aka empty bucket)
   NC:      Number of chained buckets
   ACL:     Average chaining length
   UP:      Usage percentange of hash-table
*/


#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <math.h>
#include "../GeneralHashFunctions_-_CPP/GeneralHashFunctions.h"

typedef struct
{
   unsigned int  longest_chain_len;
   unsigned int  lcl_cnt;
   unsigned int  num_zero_len;
   unsigned int  num_chaining;
   double        average_chain_len;
   double        usage_percentage;
   HashFunction  function;
   char*         hash_name;

}hf_result;



void print_result(hf_result result);
void print_stats(hf_result* result, unsigned int count);
void test_hash(hf_result* result, const std::vector < std::string >& word_list);


void read_file(const std::string file_name, std::vector<std::string>& buffer)
{
   std::ifstream in_file(file_name.c_str());
   if (!in_file)
   {
      return;
   }

   std::istream_iterator< std::string > is(in_file);
   std::istream_iterator< std::string > eof;
   std::copy( is, eof, std::back_inserter(buffer));
}

int main(int argc, char* argv[])
{

   hf_result result[] = {
                         {0, 0, 0, 0, 0.0, 0.0, RSHash  , "RSHash  "},
                         {0, 0, 0, 0, 0.0, 0.0, PJWHash , "PJWHash "},
                         {0, 0, 0, 0, 0.0, 0.0, ELFHash , "ELFHash "},
                         {0, 0, 0, 0, 0.0, 0.0, BKDRHash, "BKDRHash"},
                         {0, 0, 0, 0, 0.0, 0.0, SDBMHash, "SDBMHash"},
                         {0, 0, 0, 0, 0.0, 0.0, DJBHash , "DJBHash "},
                         {0, 0, 0, 0, 0.0, 0.0, DEKHash , "DEKHash "},
                         {0, 0, 0, 0, 0.0, 0.0, PLDHash , "PLDHash "},
                         {0, 0, 0, 0, 0.0, 0.0, APHash  , "APHash  "},
                        };

   std::cout << "Index\tHash Name\tLCL\tLCL_CNT\tNZL\tNC\t  ACL\t UP%" << std::endl;


   std::vector < std::string > word_list;

   read_file("word-list.txt", word_list);

   for(unsigned int i = 0; i < sizeof(result) / sizeof(hf_result); i++)
   {
      test_hash(&result[i], word_list);
      printf("%2d\t",i);
      print_result(result[i]);
   }

   exit(EXIT_SUCCESS);
   return true;

}


void print_result(hf_result result)
{
   std::cout <<
               result.hash_name          << "\t" <<
               result.longest_chain_len  << "\t" <<
               result.lcl_cnt            << "\t" <<
               result.num_zero_len       << "\t" <<
               result.num_chaining       << "\t" <<
               result.average_chain_len  << "\t" <<
               result.usage_percentage   << std::endl;
}


unsigned int get_next_largest_prime(unsigned int val)
{
   bool found = false;
   if (val % 2 == 0) val++;
   while (!found)
   {
      found = true;
      val += 2;
      for(unsigned int i = 3; i < (static_cast<unsigned int>(sqrt(1.0 * val)) + 1); i+=2)
      {
         if (val % i == 0)
         {
            found = false;
            break;
         }
      }
   }
   return val;
}


void test_hash(hf_result* result,const std::vector < std::string >& word_list)
{
   unsigned int  i           = 0;
   unsigned int bucket_count = get_next_largest_prime(word_list.size() * 2);
   unsigned int* bucket      = NULL;
   bucket = new unsigned int[bucket_count];

   for(i = 0; i < bucket_count; i++) bucket[i] = 0;

   for(i = 0; i < word_list.size(); i++)
   {
      bucket[result->function(word_list[i]) % bucket_count]++;
   }

   for (i = 0; i < bucket_count; i++)
   {
      if (bucket[i] == 0)
        result->num_zero_len++;
      else
      {
         if (bucket[i] > result->longest_chain_len)
         {
           result->longest_chain_len = bucket[i];
           result->lcl_cnt = 1;
         }
         else if (bucket[i] == result->longest_chain_len)
           result->lcl_cnt++;

         result->num_chaining++;
         result->average_chain_len += bucket[i];
      }
   }

   result->average_chain_len /= (double)result->num_chaining;
   result->usage_percentage =  ((double)result->num_chaining / (double)bucket_count) * 100.0;

   delete [] bucket;

}
