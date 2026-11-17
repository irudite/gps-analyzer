#include <iostream>
#include <vector>
#include <tinyxml2.h>

struct GpsPoint {
    double latitude;
    double longitude;
};

int main(int argc, char **argv) {
    // error handle invalid command line arguments passed through
    if (argc != 2) {
        std::cerr << "Usage: gps-analyzer <file-name>\n"; 
        return -1;
    }

    // read the gpx file, and if it doesn't exist return error message
    tinyxml2::XMLDocument doc; 
    if (doc.LoadFile(argv[1]) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Could not read gpx file: " << doc.ErrorStr() << '\n';
        return -1;
    }

    // jump to first track segment
    const auto* gpx = doc.FirstChildElement("gpx");     
    const auto* track = gpx ? gpx->FirstChildElement("trk") : nullptr;
    const auto* segment = track ? track->FirstChildElement("trkseg") : nullptr;

    if (segment == nullptr) {
        std::cerr << "No track segment found in file\n";
        return -1;
    }

    // iterate through all recorded gps points w/i a segment
    std::vector<GpsPoint> points;
    for (const auto* node = segment->FirstChildElement("trkpt"); 
                     node != nullptr;
                     node = segment->NextSiblingElement("trkpt")) {
        GpsPoint point{}; 
        if (node->QueryDoubleAttribute("lat", &point.latitude) != tinyxml2::XML_SUCCESS ||
                node->QueryDoubleAttribute("lon", &point.longitude) != tinyxml2::XML_SUCCESS) {
            std::cerr << "Invalid coordinate\n";
            return -1;
        }
        std::cout << point.latitude;
        points.push_back(point);
    }

    if (points.empty()) {
        std::cerr << "Track segment has no points\n";
        return -1;
    }

    const GpsPoint& first = points.front();
    std::cout << "Read " << points.size() << " GPS points.\n"
              << "First point: " << first.latitude << ", "
              << first.longitude << '\n';

    return 0;
}
