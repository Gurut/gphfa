/*
 **************************************************************************
 *                                                                        *
 *          General Purpose Hash Function Algorithms Library              *
 *                                                                        *
 * Author: Arash Partow - 2002                                            *
 * Class: Bloom Filter                                                    *
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



#ifndef INCLUDE_BLOOM_FILTER_H
#define INCLUDE_BLOOM_FILTER_H

#include <iostream>
#include <vector>
#include "../GeneralHashFunctions_-_CPP/GeneralHashFunctions.h"


const unsigned int char_size = 0x08;    // 8 bits in 1 char(unsigned)

const unsigned char bit_mask[8] = {
                                   0x01, //00000001
                                   0x02, //00000010
                                   0x04, //00000100
                                   0x08, //00001000
                                   0x10, //00010000
                                   0x20, //00100000
                                   0x40, //01000000
                                   0x80  //10000000
                                 };

class bloom_filter
{

public:

   bloom_filter(unsigned int tbl_size)
   {
      table_size = tbl_size;
      hash_table = new unsigned char[table_size];
      register_default_hash_functions();
   }

   ~bloom_filter()
   {
      delete[] hash_table;
   }

   void register_default_hash_functions()
   {
      hash_function.push_back(RSHash  );
      hash_function.push_back(JSHash  );
      hash_function.push_back(PJWHash );
      hash_function.push_back(BKDRHash);
      hash_function.push_back(SDBMHash);
      hash_function.push_back(DJBHash );
      hash_function.push_back(DEKHash );
      hash_function.push_back(APHash  );
   }

   void insert(const std::string key)
   {
      for(std::size_t i = 0; i < hash_function.size(); i++)
      {
         unsigned int hash = hash_function[i](key) % (table_size * char_size);
         hash_table[hash / char_size] |= bit_mask[hash % char_size];
      }
   }

   bool contains(const std::string key)
   {
      for(std::size_t i = 0; i < hash_function.size(); i++)
      {
         unsigned int hash = hash_function[i](key) % (table_size * char_size);
         unsigned int bit  = hash % char_size;
         if ((hash_table[hash / char_size] & bit_mask[bit]) != bit_mask[bit])
         {
            return false;
         }
      }
      return true;
   }

private:

   std::vector < HashFunction > hash_function;
   unsigned char*               hash_table;
   unsigned int                 table_size;

};

#endif
