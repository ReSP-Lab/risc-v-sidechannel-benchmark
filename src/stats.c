/*
* This file provides statistical functions to obtain Welch t-test p values
*
* Copyright (C) 2024-2025 Cédrick Austa <cedrick.austa@ulb.be>
* Copyright (C) 2020 Shuwen Deng <shuwen.deng@yale.edu>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*
*/

#include "stats.h"

double Pvalue (const double *restrict ARRAY1, const size_t ARRAY1_SIZE, const double *restrict ARRAY2, const size_t ARRAY2_SIZE) {
    if (ARRAY1_SIZE <= 1) {
        return 1.0;
    } else if (ARRAY2_SIZE <= 1) {
        return 1.0;
    }
    double fmean1 = 0.0, fmean2 = 0.0;

    // get sum of values in ARRAY1
    for (size_t x = 0; x < ARRAY1_SIZE; x++) {
        // check to make sure this is a real number
        if (isfinite(ARRAY1[x]) == 0) {
            puts("Got a non-finite number in 1st array, can't calculate P-value.");
            exit(EXIT_FAILURE);
        }
        fmean1 += ARRAY1[x];
    }
    fmean1 /= ARRAY1_SIZE;

    //get sum of values in ARRAY2
    for (size_t x = 0; x < ARRAY2_SIZE; x++) {
        //check to make sure this is a real number
        if (isfinite(ARRAY2[x]) == 0) {
            puts("Got a non-finite number in 2nd array, can't calculate P-value.");
            exit(EXIT_FAILURE);
        }
        fmean2 += ARRAY2[x];
    }
    fmean2 /= ARRAY2_SIZE;

    // printf("mean1 = %lf mean2 = %lf\n", fmean1, fmean2);
    // if the means are equal, the p-value is 1, leave the function
    if (fmean1 == fmean2) {
        printf("the means are equal\n");
        return 1.0;
    }
    double unbiased_sample_variance1 = 0.0, unbiased_sample_variance2 = 0.0;

    //1st part of added unbiased_sample_variance
    for (size_t x = 0; x < ARRAY1_SIZE; x++)
    {
        unbiased_sample_variance1 += (ARRAY1[x]-fmean1)*(ARRAY1[x]-fmean1);
    }
    for (size_t x = 0; x < ARRAY2_SIZE; x++)
    {
        unbiased_sample_variance2 += (ARRAY2[x]-fmean2)*(ARRAY2[x]-fmean2);
    }
    // printf("unbiased_sample_variance1 = %%lf\\tunbiased_sample_variance2 = %%lf\n",unbiased_sample_variance1,unbiased_sample_variance2);//DEBUGGING
    unbiased_sample_variance1 = unbiased_sample_variance1/(ARRAY1_SIZE-1);
    unbiased_sample_variance2 = unbiased_sample_variance2/(ARRAY2_SIZE-1);
    const double WELCH_T_STATISTIC = (fmean1-fmean2)/sqrt(unbiased_sample_variance1/ARRAY1_SIZE+unbiased_sample_variance2/ARRAY2_SIZE);
    const double DEGREES_OF_FREEDOM = pow((unbiased_sample_variance1/ARRAY1_SIZE+unbiased_sample_variance2/ARRAY2_SIZE),2.0)//numerator
    /
    (
        (unbiased_sample_variance1*unbiased_sample_variance1)/(ARRAY1_SIZE*ARRAY1_SIZE*(ARRAY1_SIZE-1))+
        (unbiased_sample_variance2*unbiased_sample_variance2)/(ARRAY2_SIZE*ARRAY2_SIZE*(ARRAY2_SIZE-1))
    );
//  printf("Welch = %%lf DOF = %%lf\n", WELCH_T_STATISTIC, DEGREES_OF_FREEDOM);
    const double a = DEGREES_OF_FREEDOM/2;
    double value = DEGREES_OF_FREEDOM/(WELCH_T_STATISTIC*WELCH_T_STATISTIC+DEGREES_OF_FREEDOM);
    if ((isinf(value) != 0) || (isnan(value) != 0)) {
        printf("free degree1\n");
        return 1.0;
    }
    if ((isinf(value) != 0) || (isnan(value) != 0)) {
        printf("free degree2\n");
        return 1.0;
    }
    const double beta = lgammal(a)+0.57236494292470009-lgammal(a+0.5);
    const double acu = 0.1E-14;
    double ai;
    double cx;
    int indx;
    int ns;
    double pp;
    double psq;
    double qq;
    double rx;
    double temp;
    double term;
    double xx;

    //  ifault = 0;
    // Check the input arguments.
    if ( (a <= 0.0)) {// || (0.5 <= 0.0 )){
    //    *ifault = 1;
    //    return value;
    }
    if ( value < 0.0 || 1.0 < value )
    {
    //    *ifault = 2;
        return value;
    }

    /*
        Special cases.
    */
    if ( value == 0.0 || value == 1.0 ) {
        return value;
    }
    psq = a + 0.5;
    cx = 1.0 - value;

    if ( a < psq * value )
    {
        xx = cx;
        cx = value;
        pp = 0.5;
        qq = a;
        indx = 1;
    }
    else
    {
        xx = value;
        pp = a;
        qq = 0.5;
        indx = 0;
    }

    term = 1.0;
    ai = 1.0;
    value = 1.0;
    ns = ( int ) ( qq + cx * psq );

    /*
        Use the Soper reduction formula.
    */
    rx = xx / cx;
    temp = qq - ai;
    if ( ns == 0 )
    {
        rx = xx;
    }

    for ( ; ; )
    {
        term = term * temp * rx / ( pp + ai );
        value = value + term;
        temp = fabs ( term );

        if ( temp <= acu && temp <= acu * value )
        {
            value = value * exp ( pp * log ( xx )
            + ( qq - 1.0 ) * log ( cx ) - beta ) / pp;

            if ( indx )
            {
                value = 1.0 - value;
            }
            break;
        }
        ai = ai + 1.0;
        ns = ns - 1;

        if ( 0 <= ns )
        {
            temp = qq - ai;
            if ( ns == 0 )
            {
                rx = xx;
            }
        }
        else
        {
            temp = psq;
            psq = psq + 1.0;
        }
    }
    return value;
}
