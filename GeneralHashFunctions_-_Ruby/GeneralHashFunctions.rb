#
#**************************************************************************
#*                                                                        *
#*          General Purpose Hash Function Algorithms Library              *
#*                                                                        *
#* Author: Arash Partow - 2002                                            *
#* URL: http://www.partow.net                                             *
#* URL: http://www.partow.net/programming/hashfunctions/index.html        *
#*                                                                        *
#* Copyright notice:                                                      *
#* Free use of the General Purpose Hash Function Algorithms Library is    *
#* permitted under the guidelines and in accordance with the most current *
#* version of the Common Public License.                                  *
#* http://www.opensource.org/licenses/cpl.php                             *
#*                                                                        *
#**************************************************************************
#


module GeneralHashFunctions

  def self.rs( str, len=str.length )
    a    = 63689
    b    = 378551
    hash = 0
    len.times{ |i|
      hash = hash * a + str[i]
      a *= b
    }
    hash & 0x7FFFFFFF
  end

  def self.js( str, len=str.length )
    hash = 1315423911
    len.times{ |i|
      hash ^= ( ( hash << 5 ) + str[i] + ( hash >> 2 ) )
    }
    hash & 0x7FFFFFFF
  end

  def self.elf( str, len=str.length )
    hash = 0
    x = 0
    len.times{ |i|
      hash = (hash << 4) + str[i]
      if  (x = hash & 0xF0000000) != 0
        hash ^= (x >> 24)
        hash &= ~x
      end
    }
    hash & 0x7FFFFFFF
  end

  def self.bkdr( str, len=str.length )
    seed = 131    # 31 131 1313 13131 131313 etc..
    hash = 0
    len.times{ |i|
      hash = ( hash * seed ) + str[i]
    }
    hash & 0x7FFFFFFF
  end

  def self.sdbm( str, len=str.length )
    hash = 0
    len.times{ |i|
      hash = str[i] + ( hash << 6 ) + ( hash << 16 ) - hash
    }
    hash & 0x7FFFFFFF
  end

  def self.djb( str, len=str.length )
    hash = 5381
    len.times{ |i|
      hash = ((hash << 5) + hash) + str[i]
    }
    hash & 0x7FFFFFFF
  end

  def self.ap( str, len=str.length )
    hash = 0
    len.times{ |i|
      if (i & 1) == 0
        hash ^= (hash << 7) ^ str[i] ^ (hash >> 3)
      else
        hash ^= ~( (hash << 11) ^ str[i] ^ (hash >> 5) )
      end
    }
    hash & 0x7FFFFFFF
  end

end