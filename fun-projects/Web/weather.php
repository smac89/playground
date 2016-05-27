<?php
    define('APIKEY', '9b70d66abb24431dd9d6012a3f8f2f7e');

    if ($argc < 2) {
        $argv[] = get_user_location();
    }

    show_weather_info(array_slice($argv, 1));

    function show_weather_info(array $locs) {
        $latlonendpoint = 'https://maps.google.com/maps/api/geocode/json?address=%s&sensor=false';

        $weatherendpoint = 'https://api.forecast.io/forecast/%s/%s,%s?%s';

        foreach ($locs as $address) {
            $loc_url = sprintf("$latlonendpoint", $address);
            $location=get_json($loc_url);

            $lat = $location->results[0]->geometry->location->lat;
            $lon = $location->results[0]->geometry->location->lng;

            $weather_url = sprintf("$weatherendpoint", APIKEY, $lat, $lon, 'exclude=[flags,hourly]');
            $weather=get_json($weather_url);

            // Print the summary
            fprintf(STDOUT, "\nThe weather summary for ($address) is: \n\"%s\". ".
                "\nThe temperature in celcius is %.2f°. " .
                "\nThe temperature in fahrenheit is %.2f°. \n",
                
                $weather->daily->summary,
                ($weather->currently->temperature - 32) * 5 / 9,
                $weather->currently->temperature);
        }
    }

    function get_json($url) {
        $content = file_get_contents($url);
        $to_json = json_decode($content);
        return $to_json;
    }

    function get_user_location() {
        $details = get_json("http://ipinfo.io/json");
        $city = $details->city ? $details->city : $details->region;
        return "$city,$details->country";
    }    
?>
