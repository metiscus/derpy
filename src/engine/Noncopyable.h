#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

// The idea for this class comes from the boost library

class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
    
private:
      noncopyable( const noncopyable& ); // intentionally undefined
      noncopyable& operator=( const noncopyable& ); // intentionally undefined
};


#endif /* NONCOPYABLE_H_ */
