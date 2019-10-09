#ifndef NAMES_H
#define NAMES_H

#include <string>

enum
{
	ent_Dale_Bearnhardt_Jr,

	ent_Jimmie_Flohnson,

	ent_Kyle_Musch,

	ent_Jeff_Bordon,

	ent_Danica_Hatrick,

	ent_Slow_Pete_Mckee
};

inline std::string GetNameOfEntity(int n)
{
	switch(n)
	{
	case ent_Dale_Bearnhardt_Jr:

		return "Dale Bearnhardt Jr";

	case ent_Jimmie_Flohnson:

		return "Jimmie Flohnson";

	case ent_Kyle_Musch:

		return "Kyle Musch";

	case ent_Jeff_Bordon:

		return "Jeff Bordon";

	case ent_Danica_Hatrick:

		return "Danica Hatrick";

	case ent_Slow_Pete_Mckee:

		return "Slow Pete Mckee";

	default:

		return "UNKNOWN DRIVER";
	}
}

#endif