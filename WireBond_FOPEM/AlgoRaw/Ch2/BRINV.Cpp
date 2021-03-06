//The MIT License (MIT)
//
//Copyright (c) 2016 ZHAOZhengyi-tgx
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

// #define __cplusplus
#include "stdafx.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"

//#ifdef __cplusplus
//extern "C" {
//#endif


  //int brinv(a,n)
  //int n;
  //double a[];
int brinv(double a[],int n)
{ 
	int *is,*js,i,j,k,l,u,v;
	double d,p;
	is= (int*)malloc(n*sizeof(int));
	js= (int*)malloc(n*sizeof(int));

    for (k=0; k<=n-1; k++)
	{ 
		d=0.0;
        for (i=k; i<=n-1; i++)
        for (j=k; j<=n-1; j++)
		{ 
			l=i*n+j; p=fabs(a[l]);
			if (p>d) 
			{ 
				d=p; is[k]=i; js[k]=j;
			}
		}
		if (d+1.0==1.0)
		{ 
			free(is); free(js); // printf("err**not inv\n");
			return(0);
		}
		if (is[k]!=k)
		{
			for (j=0; j<=n-1; j++)
			{ 
				u=k*n+j; v=is[k]*n+j;
				p=a[u]; a[u]=a[v]; a[v]=p;
			}
		}
		if (js[k]!=k)
		{
			for (i=0; i<=n-1; i++)
			{ 
				u=i*n+k; v=i*n+js[k];
				p=a[u]; a[u]=a[v]; a[v]=p;
			}
		}
		l=k*n+k;
		a[l]=1.0/a[l];
		for (j=0; j<=n-1; j++)
		{
			if (j!=k)
			{ 
				u=k*n+j; a[u]=a[u]*a[l];
			}
		}

        for (i=0; i<=n-1; i++)
		{
			if (i!=k)
			{
				for (j=0; j<=n-1; j++)
				{
					if (j!=k)
					{ u=i*n+j;
					a[u]=a[u]-a[i*n+k]*a[k*n+j];
					}
				}
			}
		}
        for (i=0; i<=n-1; i++)
		{
			if (i!=k)
			{ 
				u=i*n+k; a[u]=-a[u]*a[l];
			}
		}
	}
    for (k=n-1; k>=0; k--)
	{ 
		if (js[k]!=k)
		{
			for (j=0; j<=n-1; j++)
			{ 
				u=k*n+j; v=js[k]*n+j;
				p=a[u]; a[u]=a[v]; a[v]=p;
            }
		}
		if (is[k]!=k)
		{
			for (i=0; i<=n-1; i++)
			{ u=i*n+k; v=i*n+is[k];
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
		}
	}
    free(is); free(js);
    return(1);
}

//#ifdef __cplusplus
//}
//#endif


