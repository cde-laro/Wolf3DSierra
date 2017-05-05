/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_countelem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-laro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/18 16:09:59 by cde-laro          #+#    #+#             */
/*   Updated: 2017/01/21 12:56:58 by cde-laro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_countelem(const char *str, char c)
{
	int		i;
	int		n;

	i = 0;
	n = 0;
	while (str[i] != '\n' && str[i])
	{
		while (str[i] != c && str[i])
			i++;
		while (str[i] == c && str[i])
			i++;
		n++;
	}
	return (n);
}
