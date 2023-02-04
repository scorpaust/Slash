#pragma once
#include "DrawDebugHelpers.h"


#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);

#define DRAW_SPHERE_SINGLE_FRAME(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);

#define DRAW_CUSTOM_COLOR_SPHERE(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, Color, true);

#define DRAW_BOX(LocationCenter, LocationExtent, Rotation, Color) if (GetWorld()) DrawDebugBox(GetWorld(), LocationCenter, LocationExtent, Rotation, Color, true, -1.f, 0, 1.f);

#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);

#define DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);

#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);

#define DRAW_POINT_SINGLE_FRAME(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f);

#define DRAW_VECTOR(World, StartLocation, EndLocation) \
	{ \
		DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
		DrawDebugPoint(World, EndLocation, 15.f, FColor::Red, true); \
	}

#define DRAW_VECTOR_SINGLE_FRAME(World, StartLocation, EndLocation) \
	{ \
		DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f); \
		DrawDebugPoint(World, EndLocation, 15.f, FColor::Red, false, -1.f); \
	}