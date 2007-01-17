/*
 **************************************************************************
 *                                                                        *
 *          General Purpose Hash Function Algorithms Test II              *
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
   NZL:     Number of zero chaining buckets (aka empty bucket)
   NC:      Number of chained buckets
   ACL:     Average chaining length
   UP:      Usage percentange of hash-table
*/


#include <stdio.h>
#include <stdlib.h>
#include "GeneralHashFunctions.h"

typedef struct
{
   unsigned int  longest_chain_len;
   unsigned int  lcl_cnt;
   unsigned int  num_zero_len;
   unsigned int  num_chaining;
   double        average_chain_len;
   double        usage_percentage;
   hash_function function;
   char*         hash_name;

}hf_result;



void print_result(hf_result result);
void test_hash_01(hf_result* result, unsigned int bucket_count, unsigned int element_count);


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
                         {0, 0, 0, 0, 0.0, 0.0, BPHash  , "BPHash  "},
                         {0, 0, 0, 0, 0.0, 0.0, FNVHash  ,"FNVHash "},
                         {0, 0, 0, 0, 0.0, 0.0, APHash  , "APHash  "},
                        };

   printf("Index\tHash Name\tLCL\tLCL_CNT\tNZL\tNC\t  ACL\t UP%%\n");


   for(unsigned int i = 0; i < sizeof(result) / sizeof(hf_result); i++)
   {
      test_hash_01(&result[i], 104729, 104700);
      printf("%2d\t",i);
      print_result(result[i]);
   }

   exit(EXIT_SUCCESS);
   return 1;

}

void print_result(hf_result result)
{
   printf("%s\t%d\t%d\t%d\t%d\t%6.2f\t%6.2f\n",
                                               result.hash_name,
                                               result.longest_chain_len,
                                               result.lcl_cnt,
                                               result.num_zero_len,
                                               result.num_chaining,
                                               result.average_chain_len,
                                               result.usage_percentage
    );
}


void test_hash_01(hf_result* result, unsigned int bucket_count, unsigned int element_count)
{

   unsigned int  i        = 0;
   char          name[25] = {0};
   unsigned int* bucket   = NULL;

   if (NULL == (bucket = (unsigned int*) malloc(bucket_count * sizeof(unsigned int))))
   {
      return;
   }

   for(i = 0; i < bucket_count; i++) bucket[i] = 0;

   for(i = 0; i < element_count; i++)
   {
      sprintf(name,"%4d4567890123456789%4d",element_count-i,i);
      name[25] = '\0';
      bucket[result->function(name,24) % bucket_count]++;
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

   free(bucket);

}
