/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-laro <cde-laro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 20:50:04 by cde-laro          #+#    #+#             */
/*   Updated: 2017/05/05 08:32:24 by cde-laro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <stdio.h>

void	mlx_textur(t_env *e, int x, int line, int hit)
{
	int 	y;
	t_intp	text;
	t_xpm	*txt;

	y = e->a.y;
	txt = get_xpm(e, hit);
	text.x = (int)(e->p->wall.x * txt->a.x);
	text.y = (int)((e->a.y - e->p->wall.y) * txt->a.y / line);
	if (y >= 0 && x >= 0 && y < WIN_Y && x < WIN_X)
	{
		e->img->bts[(y * e->img->size_line) + ((e->img->bpp / 8) * x) + 2] = txt->img->bts[(text.y * txt->img->size_line) + ((txt->img->bpp / 8) * text.x) + 2];
		e->img->bts[(y * e->img->size_line) + ((e->img->bpp / 8) * x) + 1] = txt->img->bts[(text.y * txt->img->size_line) + ((txt->img->bpp / 8) * text.x) + 1];
		e->img->bts[(y * e->img->size_line) + ((e->img->bpp / 8) * x)] = txt->img->bts[(text.y * txt->img->size_line) + ((txt->img->bpp / 8) * text.x)];
	}
}

void	calc_line_len(t_env *e, int x, int side, int hit)
{
	e->p->line_h = (int)((WIN_Y / e->p->pwd));
	e->p->draw_s.x = x;
	e->p->draw_s.y = (-e->p->line_h / 2 + WIN_Y / 2) - e->k->sneak + e->k->jump + 50;
	e->p->draw_s.y = (e->p->draw_s.y < 0 && !e->pack ? 0 : e->p->draw_s.y);
	e->p->draw_e.x = x;
	e->p->draw_e.y = e->p->line_h / 2 + WIN_Y / 2 - e->k->sneak + e->k->jump + 50;
	e->p->draw_e.y = (e->p->draw_e.y > WIN_Y ? WIN_Y : e->p->draw_e.y);
	if (side == 0)
        e->colors = (e->p->step.x == -1 ? BLUE : GREEN);
    else
        e->colors = (e->p->step.y == -1 ? YELLOW : RED);
    e->a.x = x;
    e->b.x = x;
    e->a.y = e->p->draw_s.y;
    e->b.y = e->p->draw_e.y;
    e->p->wall.y = e->a.y;
    while (e->a.y <= WIN_Y)
    {
        if (e->a.y < e->b.y && e->a.y > 0)
        {
            if (e->pack)
                mlx_textur(e, x, e->p->line_h, hit);
            else
                pix_put_img(e, x, e->a.y, e->colors);
        }
        else
            pix_put_img(e, x, e->a.y, GREY);
        e->a.y += 1;
    }
}

void	find_wall(t_env *e, int hit, int x, int side)
{
	//hit = (e->map->data[(int)e->p->map.y][(int)e->p->map.x] > 0 ? 1 : 0);
	while (hit == 0)
	{
		if (e->p->sided.x < e->p->sided.y)
		{
			e->p->sided.x += e->p->deltad.x;
			e->p->map.x += e->p->step.x;
			side = 0;
		}
		else
		{
			e->p->sided.y += e->p->deltad.y;
			e->p->map.y += e->p->step.y;
			side = 1;
		}
		if (e->p->map.y > e->map->maxy || e->p->map.x > e->map->maxx ||
			e->p->map.x < 0 || e->p->map.x < 0)
			print_error_code(12);
		hit = e->map->data[(int)e->p->map.y][(int)e->p->map.x];
	}
	if (side == 0)
	{
		e->p->pwd = (e->p->map.x - e->p->rayp.x + (1 - e->p->step.x) / 2)
			/ e->p->rayd.x;
		e->p->wall.x = e->p->rayp.y + e->p->pwd * e->p->rayd.y;
	}
	else
	{
		e->p->pwd = (e->p->map.y - e->p->rayp.y + (1 - e->p->step.y) / 2)
			/ e->p->rayd.y;
		e->p->wall.x = e->p->rayp.x + e->p->pwd * e->p->rayd.x;
	}
	e->p->wall.x -= (int)(e->p->wall.x);
	calc_line_len(e, x, side, hit);
}

void	draw_column(t_env *e, int x)
{
	e->p->deltad.x = sqrt(1 + SQ(e->p->rayd.y) / SQ(e->p->rayd.x));
	e->p->deltad.y = sqrt(1 + SQ(e->p->rayd.x) / SQ(e->p->rayd.y));
	e->p->step.x = (e->p->rayd.x < 0 ? -1 : 1);
	e->p->step.y = (e->p->rayd.y < 0 ? -1 : 1);
	if (e->p->rayd.x < 0)
		e->p->sided.x = (e->p->rayp.x - e->p->map.x) * e->p->deltad.x;
	else
		e->p->sided.x = (e->p->map.x + 1 - e->p->rayp.x) * e->p->deltad.x;
	if (e->p->rayd.y < 0)
		e->p->sided.y = (e->p->rayp.y - e->p->map.y) * e->p->deltad.y;
	else
		e->p->sided.y = (e->p->map.y + 1 - e->p->rayp.y) * e->p->deltad.y;
	find_wall(e, 0, x, 0);
}

void	draw_frame(t_env *e)
{
	int		x;

	x = -1;
	while (++x < WIN_X)
	{
		e->p->rat = 2 * (double)(x) / (double)(WIN_X) - 1;
		e->p->rayp.x = e->p->pos.x;
		e->p->rayp.y = e->p->pos.y;
		e->p->rayd.x = e->p->dir.x + e->p->plane.x * e->p->rat;
		e->p->rayd.y = e->p->dir.y + e->p->plane.y * e->p->rat;
		e->p->map.x = (int)e->p->rayp.x;
		e->p->map.y = (int)e->p->rayp.y;
		draw_column(e, x);
	}
}

void	start(t_env *e)
{

	e->p = (t_player *)malloc(sizeof(t_player));
	if (e->map->data[e->map->maxy / 2][e->map->maxx / 2])
	{
		ft_putendl("The center must be empty");
		exit(-1);
	}
	e->p->pos.x = e->map->maxx / 2;
	e->p->pos.y = e->map->maxy / 2;
	e->p->dir.x = -0.5;
	e->p->dir.y = 0;
	e->p->plane.x = 0;
	e->p->plane.y = 0.66;
	e->p->speed = 0.3;
	e->p->r_s = 0.1;
	e->p->crossy = 40;
	e->p->ammo = DEF_AMMO;
	e->p->ammo_tick = 0;
}
