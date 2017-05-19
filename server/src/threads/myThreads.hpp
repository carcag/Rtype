#ifndef    _MYTHREADS_HPP_
# define  _MYTHREADS_HPP_

# include  "iThread.hpp"

# ifdef    WIN32
#  include  "wThread.hpp"
# else
#  include  "uThread.hpp"
# endif    /* _WIN32_ */

#endif    /* _MYTHREADS_HPP_ */ 
