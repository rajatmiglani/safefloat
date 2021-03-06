#ifndef BOOST_SAFE_FLOAT_POLICY_CHECK_SUBTRACTION_INEXACT_HPP
#define BOOST_SAFE_FLOAT_POLICY_CHECK_SUBTRACTION_INEXACT_HPP

#include <boost/safe_float/policy/check_base_policy.hpp>

#ifdef FENV_AVAILABLE
#pragma STDC FENV_ACCESS ON
#include <fenv.h>
#endif

namespace boost {
namespace safe_float{
namespace policy{

template<class FP>
class check_subtraction_inexact : public check_policy<FP> {
#ifndef FENV_AVAILABLE
    FP prev_l=0;
    FP prev_r=0;
#endif
public:
    virtual bool pre_subtraction_check(const FP& lhs, const FP& rhs){
#ifndef FENV_AVAILABLE
        prev_l = lhs;
        prev_r = rhs;
        return true;
#else
        return ! std::feclearexcept(FE_INEXACT);
#endif
    }

    virtual bool post_subtraction_check(const FP& rhs){
#ifndef FENV_AVAILABLE
        return ((rhs + prev_r) == prev_l) && ((prev_l - rhs) == prev_r); //this check is not completely safe, need to do some math to get a proper implementation...
#else
        return ! std::fetestexcept(FE_INEXACT);
#endif
    }

    virtual std::string subtraction_failure_message(){
        return std::string("Non reversible subtraction applied");
    }

};

}
}
}

#endif // BOOST_SAFE_FLOAT_POLICY_CHECK_SUBTRACTION_INEXACT_HPP

