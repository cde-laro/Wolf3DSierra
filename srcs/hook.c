/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-laro <cde-laro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/27 04:33:36 by cde-laro          #+#    #+#             */
/*   Updated: 2017/05/05 08:31:11 by cde-laro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <stdio.h>

int		mouse_funct(int button,int x,int y, t_env *e)
{
	(void)x;
	(void)y;
	(void)e;
	if (button == 1 && e->p->ammo_tick == 0)
	{
		if (e->p->ammo != 0)
		{
			if (!e->k->sneak)
				e->p->crossy += 11;
			system("afplay resources/sounds/fire.mp3 &");
			e->p->ammo--;
		}
		else
			system("afplay resources/sounds/empty.mp3 &");
	}
	return (0);
}

int		game_loop(t_env *e)
{
	print_map(e, e->map->data, e->map->maxx, e->map->maxy);
	static int	ticks = 0;

	e->p->speed = (e->k->sprint ? DEF_SPEED * 2 : DEF_SPEED);
	e->p->speed = (e->k->sneak ? DEF_SPEED / 2 : e->p->speed);
	if (e->k->right)
		rotate(e, -1);
	if (e->k->left)
		rotate(e, 1);
	if (e->k->up && e->k->jump_state == 0)
		move(e, 1);
	if (e->k->down && e->k->jump_state == 0)
		move(e, -1);
	jump_dec(e);
	e->p->ammo_tick -= (e->p->ammo_tick == 0 ? 0 : 1);
	printf("Ammo = %d, ammo-tick = %d\n", e->p->ammo, e->p->ammo_tick);
	e->p->ammo = (e->p->ammo_tick == 1 ? DEF_AMMO : e->p->ammo);
	ticks++;
	if (!e->k->sneak)
		e->p->crossy *= 0.9999;
	if (ticks % 2 == 0 && e->k->rotation)
		e->k->dec.x--;
	reprint(e);
	if (e->p->ammo_tick != 0)
		mlx_string_put (e->mlx, e->win, 10, WIN_Y - 50, RED, "RELOADING");
	else
		mlx_string_put (e->mlx, e->win, 10, WIN_Y - 50, (e->p->ammo == 0 ? RED : 0x00FFFFFF), ft_itoa(e->p->ammo));
	return (0);
}

int		key_press(int k, t_env *e)
{
	if (k == KEY_RIGHT || k == KEY_D)
		e->k->right = 1;
	else if (k == KEY_LEFT || k == KEY_A)
		e->k->left = 1;
	else if (k == KEY_UP || k == KEY_W)
		e->k->up = 1;
	else if (k == KEY_DOWN || k == KEY_S)
		e->k->down = 1;
	else if (k == KEY_SHIFT_LEFT)
		e->k->sprint = 1;
	else if (k == KEY_T)
		e->pack = (e->pack == TXT_PACK ? 0 : e->pack + 1);
	else if (k == KEY_Z)
	{
		e->p->crossy = 100;
		e->k->sneak = 100;
		reprint(e);
	}
	else if (k == KEY_R)
	{
		if (e->p->ammo_tick == 0)
		{
			ft_putendl("lel");
			system("afplay resources/sounds/reload.mp3 &");
			e->p->ammo_tick = 40;
		}
	}
	else if (k == KEY_SPACEBAR)
		e->k->jump_state = (!e->k->jump_state ? 1 : e->k->jump_state);
	else if (k == KEY_R)
		e->k->rotation = (e->k->rotation ? 0 : 1);
	else if (k == KEY_ESCAPE)
		exit(0);
	return (0);
}

int		key_release(int k, t_env *e)
{
	if (k == KEY_RIGHT || k == KEY_D)
		e->k->right = 0;
	else if (k == KEY_LEFT || k == KEY_A)
		e->k->left = 0;
	else if (k == KEY_UP || k == KEY_W)
		e->k->up = 0;
	else if (k == KEY_DOWN || k == KEY_S)
		e->k->down = 0;
	else if (k == KEY_SHIFT_LEFT)
		e->k->sprint = 0;
	else if (k == KEY_Z)
	{
		e->p->crossy = 0;
		e->k->sneak = 0;
		reprint(e);
	}
	return (0);
}
