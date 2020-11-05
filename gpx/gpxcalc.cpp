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

    const TrackPoint* previous = NULL;

    for (const auto& segment : track.trackSegments())
    {
        for (const auto& point : segment.trackPoints())
        {
            if (previous != NULL)
            {
                assert(point.time() >= previous->time());

                auto displacement = point.position() - previous->position();
                stream.emplace_back(point.time(), std::chrono::duration<double>(point.time() - previous->time()), point.position().altitude(), displacement.horizontal, displacement.vertical);
            }

            previous = &point;
        }
    }

    return stream;
}

}   // namespace gpx