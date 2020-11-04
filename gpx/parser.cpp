#include "parser.h"

namespace gpx
{
namespace
{

TrackPoint parseTrackPoint(/*xmlnode*/)
{
    // TODO: parse this from XML
    double latitude = 0;
    double longitude = 0;
    double altitude = 0;
    std::time_t time = 0;

    SurfaceCoordinates coords(latitude, longitude);
    Position position(coords, altitude);
    return TrackPoint(position, time);
}

TrackSegment parseTrackSegment(/*xmlnode*/)
{
    TrackSegment segment;

    /* for each parsed track point */
    auto trackPoint = parseTrackPoint(/*xmlnode*/);
    segment.addTrackPoint(trackPoint);

    return segment;
}

Track parseTrack(/*xmlnode*/)
{
    Track track("", 0);

    /* for each parsed track sengment */
    auto segment = parseTrackSegment(/*xmlnode*/);
    track.addTrackSegment(segment);

    return track;
}

}

Activity parseFile(const std::string& gpxFileName)
{
    Activity activity;

    /* for each parsed track */
    auto track = parseTrack(/*xmlnode*/);
    activity.addTrack(track);

    return activity;
}

}