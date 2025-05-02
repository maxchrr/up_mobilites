/*
 * Ligne de Bus
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "busline.h"

int bl_getcurrent_posx(BusLine l)
{
	return bs_getposx(list_getnode(l)->bs);
}

int bl_getcurrent_posy(BusLine l)
{
	return bs_getposy(list_getnode(l)->bs);
}

BusLine bl_getnext_bs(BusLine l)
{
	if (list_is_empty(l)) return NULL;
	if (gettype(list_getnode(l)) == STATION)
	{
		if (list_is_empty(list_getnext_node(l)))
			return NULL; // Terminus
		else
			return list_getnext_node(list_getnext_node(l)); // Le suivant est une route, donc après une station
	}
	else if (gettype(list_getnode(l)) == ROUTE)
		return list_getnext_node(l); // Le suivant est une station
	else
		return NULL;
}

BusLine bl_getprev_bs(BusLine l)
{
	if (list_is_empty(l)) return NULL;
	if (gettype(list_getnode(l)) == STATION)
	{
		if (list_is_empty(list_getprev_node(l)))
			return NULL; // Terminus
		else
			return list_getprev_node(list_getprev_node(l)); // Le précédent est une route, donc avant une station
	}
	else if (gettype(list_getnode(l)) == ROUTE)
		return list_getprev_node(l); // Le précédent est une station
	else
		return NULL;
}

BusLine bl_getnext_br(BusLine l)
{
	if (list_is_empty(l)) return NULL;
	if (gettype(list_getnode(l)) != STATION)
	{
		fprintf(stdout, "Attention déjà sur une route\n");
		return NULL;
	}
	else if (gettype(list_getnode(l)) != ROUTE)
		return list_getnext_node(l); // Le suivant est une route
	else
		return NULL;
}

BusLine bl_getprev_br(BusLine l)
{
	if (list_is_empty(l)) return NULL;
	if (gettype(list_getnode(l)) != STATION)
	{
		fprintf(stdout, "Attention déjà sur une route\n");
		return NULL;
	}
	else if (gettype(list_getnode(l)) != ROUTE)
		return list_getprev_node(l); // Le précédent est une route
	else
		return NULL;
}
