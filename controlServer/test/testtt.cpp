/*
 * =====================================================================================
 *
 *       Filename:  testtt.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/22/14 16:32:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#include <stdio.h>

int main()
{
    int *pA = new int[1];

    *pA = 5;

    delete [] pA;
}
