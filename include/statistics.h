#ifndef STATISTICS__
#define STATISTICS__

namespace sstats{

struct M_STD{
  M_STD(double m, double s):mean(m), std(s){};
  double mean;
  double std;
};
M_STD mean_std(const double* arr, const int size);
M_STD median_std(const double* arr, const int size);

double std(const double* dist, const int size);
double median(const double* dist, const int size);
double mean(const double* dist, const int size);


struct MINMAX{
  MINMAX(double min, double max):min(min),max(max){};
  double min;
  double max;
};
double min(const double* dist, const int size);
double max(const double* dist, const int size);
MINMAX minmax(const double* dist, const int size);



double __quickselect(double* list, const int size, int k);
double __nlogn_median(double* list, const int size);
int __compare(const void* a, const void*b);
}

#endif
