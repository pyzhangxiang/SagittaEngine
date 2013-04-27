#include <iostream>
#include <string>

#include <alglib/optimization.h>
using namespace alglib;

#include <eigen/Eigen/Dense>
typedef Eigen::MatrixXd Matrix;
// optimization test
namespace opttest
{
    void  function1_fvec(const real_1d_array &x, real_1d_array &fi, void *ptr)
    {
        //
        // this callback calculates
        // f0(x0,x1) = 100*(x0+3)^4,
        // f1(x0,x1) = (x1-3)^4
        //
        fi[0] = 10*pow(x[0]+3,2);
        fi[1] = pow(x[1]-3,2);
    }
    void  function1_jac(const real_1d_array &x, real_1d_array &fi, real_2d_array &jac, void *ptr)
    {
        //
        // this callback calculates
        // f0(x0,x1) = 100*(x0+3)^4,
        // f1(x0,x1) = (x1-3)^4
        // and Jacobian matrix J = [dfi/dxj]
        //
        fi[0] = 10*pow(x[0]+3,2);
        fi[1] = pow(x[1]-3,2);
        jac[0][0] = 20*(x[0]+3);
        jac[0][1] = 0;
        jac[1][0] = 0;
        jac[1][1] = 2*(x[1]-3);
    }
    
    // This example demonstrates minimization of F(x0,x1) = f0^2+f1^2, where
    //
    //     f0(x0,x1) = 10*(x0+3)^2
    //     f1(x0,x1) = (x1-3)^2
    //
    // using "VJ" mode of the Levenberg-Marquardt optimizer.
    //
    // Optimization algorithm uses:
    // * function vector f[] = {f1,f2}
    // * Jacobian matrix J = {dfi/dxj}.
    //
    void test1()
    {
        real_1d_array x = "[0,0]";
        double epsg = 0.0000000001;
        double epsf = 0;
        double epsx = 0;
        ae_int_t maxits = 0;
        minlmstate state;
        minlmreport rep;
        
        minlmcreatevj(2, x, state);
        minlmsetcond(state, epsg, epsf, epsx, maxits);
        alglib::minlmoptimize(state, function1_fvec, function1_jac);
        minlmresults(state, x, rep);
        
        printf("%d\n", int(rep.terminationtype)); // EXPECTED: 4
        printf("%s\n", x.tostring(2).c_str()); // EXPECTED: [-3,+3]
    }
    
    #define ROSD 105.0
    /* Rosenbrock function, global minimum at (1, 1) */
    void ros(const real_1d_array &x, real_1d_array &fi, void *ptr)
    {

        fi[0]=((1.0-x[0])*(1.0-x[0]) + ROSD*(x[1]-x[0]*x[0])*(x[1]-x[0]*x[0]));
        fi[1]=((1.0-x[0])*(1.0-x[0]) + ROSD*(x[1]-x[0]*x[0])*(x[1]-x[0]*x[0]));
    }
    
    void jacros(const real_1d_array &x, real_1d_array &fi, real_2d_array &jac, void *ptr)
    {
        fi[0]=((1.0-x[0])*(1.0-x[0]) + ROSD*(x[1]-x[0]*x[0])*(x[1]-x[0]*x[0]));
        fi[1]=((1.0-x[0])*(1.0-x[0]) + ROSD*(x[1]-x[0]*x[0])*(x[1]-x[0]*x[0]));

        jac[0][0]=(-2 + 2*x[0]-4*ROSD*(x[1]-x[0]*x[0])*x[0]);
        jac[0][1]=(2*ROSD*(x[1]-x[0]*x[0]));
        jac[0][0]=(-2 + 2*x[0]-4*ROSD*(x[1]-x[0]*x[0])*x[0]);
        jac[0][1]=(2*ROSD*(x[1]-x[0]*x[0]));
    }
    
    void ros2(const real_1d_array &x, real_1d_array &fi, void *ptr)
    {
        
        fi[0]= x[0]*x[0] + x[1]*x[1];
        fi[1]=x[0]*x[0] + x[1]*x[1];
    }
    
    void jacros2(const real_1d_array &x, real_1d_array &fi, real_2d_array &jac, void *ptr)
    {
        fi[0]= x[0]*x[0] + x[1]*x[1];
        fi[1]=x[0]*x[0] + x[1]*x[1];
        
        jac[0][0]=2 * x[0];
        jac[0][1]=2 * x[1];
        jac[0][0]=2 * x[0];
        jac[0][1]=2 * x[1];
    }
    
    void mytestRos()
    {
        double lamda = 0.0001;
        real_1d_array x = "[-1.2,1.0]";
        real_1d_array fi = "[0, 0]";
        real_1d_array lastfi = "[1000, 1000]";
        real_2d_array jac = "[[2,3],[3,4]]";
        Matrix mjac(2, 2);
        Matrix mfi(2, 1);
        Matrix mLastfi(2, 1);
        double thr = 0.0000001;
        double err = 30000;
        unsigned int count = 0;
        while(err > thr && count < 40000)
        {
            ++count ;
            
            ros(x, fi, 0);
            jacros(x, fi, jac, 0);
            
            mfi(0, 0) = fi[0];
            mfi(1, 0) = fi[1];
            
            /*mjac[0][0] = jac[0][0];
            mjac[0][1] = jac[0][1];
            mjac[1][0] = jac[1][0];
            mjac[1][1] = jac[1][1];
            if(!mjac.inv())
            {
                std::cout << "inv failed\n";
                break;
            }
            
            Matrix dx = mjac * mfi;
            x[0] = x[0] - dx[0][0];
            x[1] = x[1] - dx[1][0];*/
            
            Matrix mmjac(1, 2);
            mmjac(0, 0) = jac[0][0];
            mmjac(0, 1) = jac[0][1];
            Matrix mmjact = mmjac.transpose();
            Matrix mmjacplus = mmjac * mmjact;
            Matrix invMMJacPlus = mmjacplus.inverse();
            
            Matrix pmmjac = mmjact * invMMJacPlus;
            
            x[0] -= fi[0] * pmmjac(0, 0);
            x[1] -= fi[1] * pmmjac(1, 0);
            
            err = fabs(double(fi[0]) - double(lastfi[0]));
            lastfi = fi;
        }
        
        printf("iter: %d, err: %f\n", count, err);
        printf("%s\n", x.tostring(2).c_str());
        
       /* Matrix tt(2, 1);
        tt[0][0] = 1;
        tt[1][0] = 9;
        Matrix tt2 = (~tt) * tt;
        std::cout << (tt2);
        if(!tt2.inv())
        {
            std::cout << "inv failed\n";
        }
        std::cout << tt2;
        Matrix tt3 = tt2 * (~tt);
        std::cout << tt3;*/
    }
    
    
    void x2(const real_1d_array &x, real_1d_array &fi, void *ptr)
    {
        
        fi[0]=x[0]*x[0];
    }
    
    void jacx2(const real_1d_array &x, real_1d_array &fi, real_2d_array &jac, void *ptr)
    {
        fi[0]=x[0]*x[0];
        
        jac[0][0]=2*x[0];
    }
    
    void mytestX2()
    {
        real_1d_array x = "[4]";
        real_1d_array fi = "[0]";
        real_1d_array lastfi = "[1000]";
        real_2d_array jac = "[[100]]";

        Matrix mjac(1, 1);
        Matrix mfi(1, 1);
        double thr = 0.00000001;
        double err = 1000;
        unsigned int count = 0;
        while(err > thr && count < 10000)
        {
            ++count ;
            
            x2(x, fi, 0);
            jacx2(x, fi, jac, 0);
            
            mfi(0, 0) = fi[0];
            
            mjac(0, 0) = jac[0][0];
            Matrix invMJac = mjac.inverse();
            
            Matrix dx = invMJac * mfi;
            x[0] = x[0] - dx(0, 0);
            
            err = fabs(double(fi[0]) - double(lastfi[0]));
            lastfi = fi;
        }
        
        printf("iter: %d, err: %f\n", count, err);
        printf("%s\n", x.tostring(2).c_str());
    }
    
    
    void testRos()
    {
        real_1d_array x = "[-1.2,1.0]";
        double epsg = 0.000000001;
        double epsf = 0;
        double epsx = 0;
        ae_int_t maxits = 0;
        minlmstate state;
        minlmreport rep;
        
        minlmcreatevj(2, x, state);
        minlmsetcond(state, epsg, epsf, epsx, maxits);
        alglib::minlmoptimize(state, ros, jacros);
        minlmresults(state, x, rep);
        
        printf("iter: %d, type: %d\n", int(rep.iterationscount), int(rep.terminationtype)); // EXPECTED: 4
        printf("%s\n", x.tostring(2).c_str()); // EXPECTED: [1,1]
    }
    
}


int main(int argc, char *argv[]){
    opttest::testRos();
	return 0;
} 
