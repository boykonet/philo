#include "philo_one.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 || *s2)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return (0);
}

int	ft_strtoi(const char *str, void **endptr)
{
	int			flag;
	long int	digit;

	flag = 1;
	digit = 0;
	if (str == NULL)
		return (0);
	while (*str && (*str == ' ' || *str == '\n' || *str == '\t'
			|| *str == '\r' || *str == '\f' || *str == '\v'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			flag = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		digit = digit * 10 + *str++ - '0';
	if (endptr)
		*endptr = (void *)str;
	return ((int)(digit * flag));
}

int	ternar_int(int condition, int p1, int p2)
{
	if (condition)
		return (p1);
	return (p2);
}
