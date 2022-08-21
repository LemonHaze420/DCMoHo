#ifndef SEED_UNIFORM_RANDOM_NUMBER_STREAM_INCLUDE
#define SEED_UNIFORM_RANDOM_NUMBER_STREAM_INCLUDE


class CSeedUniformRandomNumberStream 
{
  public:


    CSeedUniformRandomNumberStream(SINT initial_seed) ;
    SINT	GetCurrentSeed() {return mCurrentSeed ; }
    SINT	GetInitialSeed() {return mInitialSeed ; }
    SINT	Next() ;
	void	Reset() {mCurrentSeed = mInitialSeed ; }
  private:

    SINT mCurrentSeed ;
    SINT mInitialSeed ;

    // mA and mM represent the values used in the random number calculations.
    // They are used in the calculation of the prime multiplicative linear 
    // congruential random number method found in this class.

    static SINT     mA;
    static SINT		mM;
};




#endif