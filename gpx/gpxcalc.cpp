#include "gpxcalc.h"

#include "track.h"


namespace gpx
{

DataStream GpxCalculator::analyzeTrack(const Track& track)
{
    DataStream stream;

    if (track.trackSegments().empty() || track.trackSegments()[0].trackPoints().size() < 2)
    {
        return stream;
    }

    const TrackPoint* start = NULL;
    const TrackPoint* previous = NULL;
    double totalDistance = 0;

    for (const auto& segment : track.trackSegments())
    {
        for (const auto& point : segment.trackPoints())
        {
            if (start == NULL)
            {
                start = &point;
            }
            if (previous != NULL)
            {
                assert(point.time() >= previous->time());

                auto displacement = point.position() - previous->position();
                totalDistance += displacement.horizontal;
                stream.emplace_back(point.time() - start->time(), std::chrono::duration<double>(point.time() - previous->time()), totalDistance, point.position().altitude(), displacement.horizontal, displacement.vertical);
            }

            previous = &point;
        }
    }

    return stream;
}

}   // namespace gpx