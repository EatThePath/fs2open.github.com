#include "parse/sexp/LuaShared.h" 
#include "object/object.h"
#include "ship/awacs.h"
#include "ship/ship.h"
#include "network/multiutil.h"



/**
 * Determine if ship visible on radar
 * 
 * @return 0 - not visible
 * @return 1 - marginally targetable (jiggly on radar)
 * @return 2 - fully targetable
 */
int shared_is_ship_visible(const ship *viewed, ship *viewer)
{
	if(!viewer)
	{
		// if the second argument is not supplied, default to the player, per retail
		if (Game_mode & GM_MULTIPLAYER)
		{
			mprintf(("In multiplayer, is-ship-visible must have two arguments!  Defaulting to the first player.\n"));

			// to make allowances for buggy missions (such as retail), just pick the first player
			// if we actually have no valid players, viewer_shipp will be NULL, but that's ok
			for (int i = 0; i < MAX_PLAYERS; ++i)
			{
				int shipnum = multi_get_player_ship(i);
				if (shipnum >= 0)
				{
					viewer = &Ships[shipnum];
					break;
				}
			}
		}
		else
			viewer = Player_ship;
	}

	object * viewed_obj = &Objects[viewer->objnum];
	int ship_is_visible = 0;
	// get ship's *radar* visiblity
	//if (viewer)
	{
		if (ship_is_visible_by_team( viewed_obj, viewer))
		{
			ship_is_visible= 2;
		}
	}

	// only check awacs level if ship is not visible by team
	if (viewer && !ship_is_visible) {
		float awacs_level = awacs_get_level(viewed_obj, viewer);
		if (awacs_level >= 1.0f) {
			ship_is_visible = 2;
		} else if (awacs_level > 0) {
			ship_is_visible = 1;
		}
	}

	return ship_is_visible;
}