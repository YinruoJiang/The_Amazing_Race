/*
 FILE: cA3a.c
 PROJECT:assignment3A
 PROGRAMMER: Yinruo Jiang
 FIRSTVERSION: 2021-03-19
 SECONDVERSION:2021-03-22
 THIRDVERSION:2021-03-24
 DESCRIPTION:
 The functions in this file are used to find the best route from one city to another.
*/


#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;


//Functions
int getNum(void);
void displayFlight(int* depaCity, int* arriCity, int* arrivalTime);
int totalTime(int farrivalTime, int arrivalTime, int depaCity, int arriCity);
void printFinalResult(vector<int> finalRoute, vector<int> finalTime, vector<int> finalDepaTime);
map<int, map<int, vector<pair<int, int>>>> initFlightTable(void);


enum city {
	Toronto = 0,
	Atlanta,
	Austin,
	Santa_Fe,
	Denver,
	Chicago,
	Buffalo
};
vector<string> cityName = { "Toronto", "Atlanta", "Austin", "Santa_Fe", "Denver", "Chicago", "Buffalo" };
const string getCityName(int enumVal)
{
	return cityName[enumVal];
}

enum timezone {
	MST = 0,
	CST,
	EST
};
vector<string> timeZoneName = { "MST", "CST", "EST" };
const string getTimeZoneName(int enumVal)
{
	return timeZoneName[enumVal];
}

// Declare the city timezones.
vector<int> cityToTimeZone = {
	EST, //time zone for Toronto
	EST, //time zone for Altanla
	CST, //time zone for Austin
	MST, //time zone for Santa_Fe
	MST, //time zone for Denver
	CST, //time zone for Chicago
	EST, //time zone for Buffalo
};

// Function: timeDiff()
// Description: get the time difference from depature to destination
// Parameters: int depaCity: depature city
//             int arriCity: destination city
// Returns: int timediff: the time difference between two cities

int timeDiff(int depaCity, int arriCity) {
	int timeDiff = 0;
	timeDiff = cityToTimeZone.at(arriCity) - cityToTimeZone.at(depaCity);
	return timeDiff;
}

// Declare and initialize the flight table.
map<int, map<int, vector<pair<int, int>>>> flightTable = initFlightTable();

/*
* NAME : BFS
* PURPOSE : find all possible path from departure to destination
*/
class BFS {
private:
	vector<vector<int>> iniPaths; //set up the city to city

	bool isNotVisited(int t, vector<int> path);

public:
	BFS() {
		// construct a graph
		iniPaths.resize(cityName.size());
		for (map<int, map<int, vector<pair<int, int>>>>::iterator from = flightTable.begin(); from != flightTable.end(); ++from) {
			for (map<int, vector<pair<int, int>>>::iterator to = from->second.begin(); to != from->second.end(); ++to) {
				iniPaths[from->first].push_back(to->first);
			}
		}
	}
	vector<vector<int>> findAllPaths(int depaCity, int arriCity);
};


bool BFS::isNotVisited(int t, vector<int> path) {
	int size = path.size();
	for (int i = 0; i < size; i++)
		if (path[i] == t)
			return false;
	return true;
}
// Function: findAllPaths()
// Description: find all possible paths and save in the savePaths
// Parameters: int depaCity: depature city
//             int arriCity: destination city
// Returns: vector<vector> int savePaths:

vector<vector<int>> BFS::findAllPaths(int depaCity, int arriCity) {
	queue<vector<int>> paths; //store all paths
	vector<int> newPath; //store current path
	vector<vector<int>> savePaths; // store all possible paths

	newPath.push_back(depaCity);
	paths.push(newPath);

	while (!paths.empty()) {
		newPath = paths.front();
		paths.pop();
		int last = newPath[newPath.size() - 1]; // get last element in the current path

		if (last == arriCity) {
			savePaths.push_back(newPath);
			continue;
		}

		for (int count = 0; count < iniPaths[last].size(); count++) {
			if (isNotVisited(iniPaths[last][count], newPath)) {
				vector<int> copyPath(newPath);
				copyPath.push_back(iniPaths[last][count]);
				paths.push(copyPath);
			}
		}
	}
	return savePaths;
}

/*
* NAME : timeInfo
* PURPOSE : calculate the arrive time and have the format for time
*/
class timeInfo {
private:
	int depaHour = 0;
	int depaMinute = 0;
	int flyHour = 0;
	int flyMinute = 0;
	int arriHour = 0;
	int arriMinute = 0;

public:

	timeInfo();
	timeInfo(int arriTime);
	timeInfo(int depaTime, int flyTime, int depaCity, int arriCity);

	int getDepatureTime();
	int getArrivalTime();
	string toString(int hm, int depaCity, int arriCity);
	//string flightInfo();

};

timeInfo::timeInfo() {}
timeInfo::timeInfo(int arriTime) {
	arriHour = arriTime / 100;
	arriMinute = arriTime % 100;
}
// Function: timeInfo()
// Description: get arrive time 
// Parameters: int depaCity: depature city
//             int arriCity: destination city
//             int depaTime: departure time
//             int flyTime: fly time
// Returns: None
timeInfo::timeInfo(int depaTime, int flyTime, int depaCity, int arriCity) {

	depaHour = depaTime / 100;
	depaMinute = depaTime % 100;

	flyHour = flyTime / 100;
	flyMinute = flyTime % 100;

	arriHour = depaHour + flyHour + timeDiff(depaCity, arriCity);
	arriMinute = depaMinute + flyMinute;
	int arrivalTime = 0;
	if (arriMinute >= 60) {
		arriMinute -= 60;
		arriHour++;
	}
	arrivalTime = arriHour * 100 + arriMinute;

}

int timeInfo::getDepatureTime() {
	return depaHour * 100 + depaMinute;
}
int timeInfo::getArrivalTime() {
	return arriHour * 100 + arriMinute;
}

// Function: toString()
// Description: format the arrive time
// Parameters: int depaCity: depature city
//             int arriCity: destination city
//             int hm: arrive time
// Returns:	   ss.str()

string timeInfo::toString(int hm, int depaCity, int arriCity) {
	bool isAm = 0;
	bool isNextDay = 0;

	hm += timeDiff(depaCity, arriCity);

	int h = 0;
	int m = 0;
	h = hm / 100;
	m = hm % 100;

	if (h > 24) {
		h -= 24;
		isAm = true;
		isNextDay = true;
	}
	else if (h > 12) {
		h -= 12;
		isAm = false;
		isNextDay = false;
	}
	else {
		isAm = true;
		isNextDay = false;
	}

	stringstream ss;
	ss << h << ":" << setfill('0') << setw(2) << m << " " << (isAm ? "a.m. " : "p.m. ") << getTimeZoneName(cityToTimeZone[arriCity]) << (isNextDay ? " next day" : "");
	return ss.str();
}

/*
* NAME : flightInfo
* PURPOSE : find all possible path departure time and arrive time
*/
class flightInfo {
private:
	int depaCity;
	int arriCity;
	int depaTime;
	int flyTime;

public:
	timeInfo findFlight(int arrivalTime, int depaCity, int arriCity);
};
// Function: findFlight()
// Description: find all possible path departure time and arrive time
// Parameters: int depaCity: depature city
//             int arriCity: destination city
//             int arrivalTime: arrive time
// Returns:	   timeInfo a: departure time and arrive time

timeInfo flightInfo::findFlight(int arrivalTime, int depaCity, int arriCity) {
	int depaTime = 0;
	int flyTime = 0;

	for (map<int, map<int, vector<pair<int, int>>>>::iterator outer_iter = flightTable.begin(); outer_iter != flightTable.end(); ++outer_iter) {
		if (outer_iter->first == depaCity) {
			for (map<int, vector<pair<int, int>>>::iterator inner_iter = outer_iter->second.begin(); inner_iter != outer_iter->second.end(); ++inner_iter) {
				if (inner_iter->first == arriCity) {
					for (vector<pair<int, int>>::iterator depa_iter = inner_iter->second.begin(); depa_iter != inner_iter->second.end(); ++depa_iter) {
						if (arrivalTime <= depa_iter->first) {
							depaTime = depa_iter->first;
							flyTime = depa_iter->second;
							break;
						}
						else {
							depaTime = inner_iter->second.front().first + 2400;
							flyTime = inner_iter->second.front().second;
						}
					}
				}
			}
		}
	}
	timeInfo a(depaTime, flyTime, depaCity, arriCity);
	return a;
}


int main()
{
	//Declare the quit condition when quitbutton !=1
	int quitButton = 1; 
	while (quitButton == 1) {
		
	    int depaCity = 0;
	    int arriCity = 0;
		// Declare the start time 
	    int arrivalTime = 0;
	    displayFlight(&depaCity, &arriCity, &arrivalTime);
		timeInfo tt;
	    cout << "Flying from " << getCityName(depaCity) << " to " << getCityName(arriCity) << "." << endl;
	    cout << "Starting from " << getCityName(depaCity) << " at " << tt.toString(arrivalTime, depaCity, depaCity) << endl;


	    BFS a;
		// v stores all possible paths from departure to destination
	    vector<vector<int>> v = a.findAllPaths(depaCity, arriCity);
		//flyTimes stores all arrival time for each path
	    vector<vector<int>> flyTimes;
		//depaTimes stores all depature time for each path
	    vector<vector<int>> depaTimes;

	    int totalTim = 0;
		//totalTimes store all time difference between final arrival time and start time for each path
	    vector<int> totalTimes;


	    for (vector<vector<int>>::iterator row = v.begin(); row != v.end(); row++) {

	        timeInfo ti(arrivalTime);
			//currentdepaTimes stores all depature time for current path
	        vector<int> currentDepaTimes;
			//currentFlyTimes stores all arrival time for current path
	        vector<int> currentFlyTimes;

	        for (vector<int>::iterator col = row->begin(); col != row->end() - 1; col++) {
	            flightInfo b;

	            ti = b.findFlight(ti.getArrivalTime(), *col, *(col + 1));
	            currentFlyTimes.push_back(ti.getArrivalTime());
	            currentDepaTimes.push_back(ti.getDepatureTime());
	        }
			totalTim = totalTime(ti.getArrivalTime(), arrivalTime, depaCity, arriCity);
	        totalTimes.push_back(totalTim);
	        flyTimes.push_back(currentFlyTimes);
	        depaTimes.push_back(currentDepaTimes);
	    }
		//minElementIndex get the minimal element index in totalTimes 
	    int minElementIndex = min_element(totalTimes.begin(), totalTimes.end()) - totalTimes.begin(); // smallest total travel time

		//finalRoute store the shortest path
	    vector<int> finalRoute;
	    finalRoute = v.at(minElementIndex);
		//finalTime store the arrive time for each city in the shortest path
	    vector<int> finalTime;
	    finalTime = flyTimes.at(minElementIndex);
		//finalDepaTime store the departure time for each city in the shortest path
	    vector<int> finalDepaTime;
	    finalDepaTime = depaTimes.at(minElementIndex);
	    printFinalResult(finalRoute, finalTime, finalDepaTime);
	    cout << "Total travel time: " << totalTimes[minElementIndex] / 100 << ":" << setfill('0') << setw(2) << totalTimes[minElementIndex] % 100 << endl << endl;
	    cout << "Continue the search?" << endl;
	    cout << "1. Yes \t 2. No" << endl;
	    quitButton = getNum();

	}


	return 0;
}


// Function: displayFlight()
// Description: prompt user to enter departure city, destination city and start time
// Parameters: int* depaCity: departure city
//             int* arriCity: destination city
//             int* arrivalTime: arrive time
// Returns:	   None

void displayFlight(int* depaCity, int* arriCity, int* arrivalTime) {
	cout << "\t\t\tFlight Information\n" << endl;
	cout << "Departure Cities:" << endl;
	cout << " 1. Toronto" << endl;
	cout << " 2. Atlanta" << endl;
	cout << " 3. Austin" << endl;
	cout << " 4. Santa_Fe" << endl;
	cout << " 5. Denver" << endl;
	cout << " 6. Chicago" << endl;
	cout << " 7. Buffalo" << endl;
	cout << "Destination Cities:" << endl;
	cout << " 1. Toronto" << endl;
	cout << " 2. Atlanta" << endl;
	cout << " 3. Austin" << endl;
	cout << " 4. Santa_Fe" << endl;
	cout << " 5. Denver" << endl;
	cout << " 6. Chicago" << endl;
	cout << " 7. Buffalo" << endl;

	cout << "Please enter the departure city number (from 1-7):" << endl;
	*depaCity = getNum() -1;
	while (*depaCity < 0 || *depaCity>6) {

		cout << "Please re-enter the departure city number (from 1-7):" << endl;
		*depaCity = getNum()-1;
	}
	cout << "Please enter the destination city number (from 1-7):" << endl;
	*arriCity = getNum() -1;
	while (*arriCity < 0 || *arriCity>76) {

		cout << "Please re-enter the destination city number (from 1-7):" << endl;
		*arriCity = getNum()-1;
	}
	cout << "Please enter the starting time: the format is (hhmm)" << endl;
	cout << "For example, if 4:20am=> 420;if 16:20pm=>1620" << endl;
	*arrivalTime = getNum();
	while (*arrivalTime < 0 || *arrivalTime >= 2400) {

		cout << "Please re-enter the starting time:" << endl;
		*arrivalTime = getNum();
	}
}

// Function: totalTime()
// Description: calculate the total time from final arrive time to start time
// Parameters: int farrivalTime: final arrive time
//             int arrivalTime: start time
//             int depaCity: departure city
//             int arriCity: destination city
// Returns:	   int totalTime: the difference between final arrive time and start time

int totalTime(int farrivalTime, int arrivalTime, int depaCity, int arriCity) {
	int farrivalHour = 0;
	int farrivalMin = 0;
	int arrivalHour = 0;
	int arrivalMin = 0;
	int totalHour = 0;
	int totalMin = 0;
	int totalTime = 0;

	farrivalHour = farrivalTime / 100;
	farrivalMin = farrivalTime % 100;
	arrivalHour = arrivalTime / 100;
	arrivalMin = arrivalTime % 100;

	totalHour = farrivalHour - arrivalHour - timeDiff(depaCity, arriCity);

	if (farrivalMin < arrivalMin) {
		farrivalMin += 60;
		farrivalHour--;
	}
	totalMin = farrivalMin - arrivalMin;
	totalTime = totalHour * 100 + totalMin;

	return totalTime;
}

// Function: printFinalResult()
// Description: print final output
// Parameters: vector<int> finalRoute: the shortest path
//             vector<int> finalTime: the arrive time for each city
//             vector<int> finalDepaTime: the departure time for each city
// Returns:	  None

void printFinalResult(vector<int> finalRoute, vector<int> finalTime, vector<int> finalDepaTime) {
	timeInfo tt;

	for (int count = 0; count < finalRoute.size(); count++) {
		if (count == 0) {
			cout << "Leaving from " << cityName.at(finalRoute.at(count)) << " at " << tt.toString(finalDepaTime.at(count), finalRoute.at(count), finalRoute.at(count)) << " for " << cityName.at(finalRoute.at(count + 1)) << endl;
		}
		else if (count == finalRoute.size() - 1) {
			cout << "Arriving in " << cityName.at(finalRoute.at(count)) << " at " << tt.toString(finalTime.at(count - 1), finalRoute.at(count), finalRoute.at(count)) << endl;
		}
		else {
			cout << "Arriving in " << cityName.at(finalRoute.at(count)) << " at " << tt.toString(finalTime.at(count - 1), finalRoute.at(count), finalRoute.at(count)) << endl;
			cout << "Leaving from " << cityName.at(finalRoute.at(count)) << " at " << tt.toString(finalDepaTime[count], finalRoute.at(count), finalRoute.at(count)) << " for " << cityName.at(finalRoute.at(count + 1)) << endl;
		}
	}
}

// Function: getNum()
// Description: Gets a int variable from the user.  If the user enters invalid input (e.g. a word),
//              -1 is returned.  Limitation: You can't tell if the user entered invalid input or truly
//              entered -1.
// Parameters: none
// Returns: the int value entered by the user or -1 if input is invalid

#pragma warning(disable: 4996)
int getNum(void)
{
	/* the array is 121 bytes in size; we'll see in a later lecture how we can
	improve this code */
	char record[121] = { 0 }; /* record stores the string */
	int number = 0;
	/* NOTE to student: indent and brace this function consistent with your
   others */
   /* use fgets() to get a string from the keyboard */
	fgets(record, 121, stdin);

	/* extract the number from the string; sscanf() returns a number
	* corresponding with the number of items it found in the string */
	if (sscanf(record, "%d", &number) != 1)
	{
		/* if the user did not enter a number recognizable by
		* the system, set number to -1 */
		number = -1;
	}
	return number;
}

// Information for all flights

map<int, map<int, vector<pair<int, int>>>> initFlightTable() {
	map<int, map<int, vector<pair<int, int>>>> flightTable;

	map<int, vector<pair<int, int>>> torontoFlight;

	vector<pair<int, int>> torAtltime; //Toronto to Atlanta Flight time
	torAtltime.push_back(make_pair(625, 220));
	torAtltime.push_back(make_pair(910, 450));
	torAtltime.push_back(make_pair(1230, 415));
	torAtltime.push_back(make_pair(2000, 215));
	torontoFlight[city::Atlanta] = torAtltime;

	vector<pair<int, int>> torDentime; //Toronto to Denver Flight time
	torDentime.push_back(make_pair(730, 335));
	torDentime.push_back(make_pair(1500, 600));
	torontoFlight[city::Denver] = torDentime;

	vector<pair<int, int>> torChitime; //Toronto to Chicago Flight time
	torChitime.push_back(make_pair(640, 120));
	torChitime.push_back(make_pair(740, 135));
	torChitime.push_back(make_pair(840, 135));
	torChitime.push_back(make_pair(940, 135));
	torChitime.push_back(make_pair(1040, 135));
	torChitime.push_back(make_pair(1140, 135));
	torChitime.push_back(make_pair(1240, 135));
	torChitime.push_back(make_pair(1340, 135));
	torChitime.push_back(make_pair(1440, 135));
	torChitime.push_back(make_pair(1530, 145));
	torChitime.push_back(make_pair(1630, 145));
	torChitime.push_back(make_pair(1730, 145));
	torChitime.push_back(make_pair(1830, 145));
	torChitime.push_back(make_pair(1930, 145));
	torChitime.push_back(make_pair(2100, 130));
	torChitime.push_back(make_pair(2200, 115));
	torontoFlight[city::Chicago] = torChitime;

	flightTable[city::Toronto] = torontoFlight;


	map<int, vector<pair<int, int>>> atlantaFlight;

	vector<pair<int, int>> atlTortime; //Atlanta to Toronto Flight time
	atlTortime.push_back(make_pair(710, 210));
	atlTortime.push_back(make_pair(1030, 410));
	atlTortime.push_back(make_pair(1500, 350));
	atlTortime.push_back(make_pair(1710, 610));
	atlTortime.push_back(make_pair(2100, 220));
	atlantaFlight[city::Toronto] = atlTortime;

	vector<pair<int, int>> atlAustime; //Atlanta to Austin Flight time
	atlAustime.push_back(make_pair(900, 210));
	atlAustime.push_back(make_pair(1530, 250));
	atlAustime.push_back(make_pair(2000, 230));
	atlantaFlight[city::Austin] = atlAustime;

	vector<pair<int, int>> atlDentime; //Atlanta to Denver Flight time
	atlDentime.push_back(make_pair(600, 300));
	atlDentime.push_back(make_pair(1320, 500));
	atlDentime.push_back(make_pair(1710, 250));
	atlantaFlight[city::Denver] = atlDentime;


	vector<pair<int, int>> atlChitime; //Atlanta to Chicago Flight time
	atlChitime.push_back(make_pair(650, 210));
	atlChitime.push_back(make_pair(750, 300));
	atlChitime.push_back(make_pair(850, 300));
	atlChitime.push_back(make_pair(950, 300));
	atlChitime.push_back(make_pair(1050, 300));
	atlChitime.push_back(make_pair(1150, 300));
	atlChitime.push_back(make_pair(1250, 300));
	atlChitime.push_back(make_pair(1350, 300));
	atlChitime.push_back(make_pair(1450, 300));
	atlChitime.push_back(make_pair(1550, 230));
	atlChitime.push_back(make_pair(1650, 230));
	atlChitime.push_back(make_pair(1750, 230));
	atlChitime.push_back(make_pair(1850, 230));
	atlChitime.push_back(make_pair(1950, 230));
	atlChitime.push_back(make_pair(2030, 210));

	atlantaFlight[city::Chicago] = atlChitime;

	flightTable[city::Atlanta] = atlantaFlight;



	map<int, vector<pair<int, int>>> austinFlight;

	vector<pair<int, int>> ausAtltime; //Austin to Atlanta Flight time
	ausAtltime.push_back(make_pair(910, 220));
	ausAtltime.push_back(make_pair(1500, 220));
	ausAtltime.push_back(make_pair(2130, 230));
	austinFlight[city::Atlanta] = ausAtltime;

	vector<pair<int, int>> ausDentime; //Austin to Denver Flight time
	ausDentime.push_back(make_pair(1030, 220));
	ausDentime.push_back(make_pair(1820, 220));
	austinFlight[city::Denver] = ausDentime;

	vector<pair<int, int>> ausSantime; //Austin to Santa_Fe Flight time
	ausSantime.push_back(make_pair(1700, 055));
	austinFlight[city::Santa_Fe] = ausSantime;


	flightTable[city::Austin] = austinFlight;



	map<int, vector<pair<int, int>>> santafeFlight;

	vector<pair<int, int>> sanAustime; //Santa_Fe to Austin Flight time
	sanAustime.push_back(make_pair(1500, 045));
	santafeFlight[city::Austin] = ausAtltime;


	flightTable[city::Santa_Fe] = santafeFlight;



	map<int, vector<pair<int, int>>> denverFlight;

	vector<pair<int, int>> denTortime; //Denver to Toronto Flight time
	denTortime.push_back(make_pair(630, 410));
	denTortime.push_back(make_pair(1030, 520));
	denTortime.push_back(make_pair(1400, 500));
	denverFlight[city::Toronto] = denTortime;

	vector<pair<int, int>> denAtltime; //Denver to Atlanta Flight time
	denAtltime.push_back(make_pair(600, 310));
	denAtltime.push_back(make_pair(1300, 320));
	denAtltime.push_back(make_pair(1500, 350));
	denverFlight[city::Atlanta] = denAtltime;

	vector<pair<int, int>> denAustime; //Denver to Austin Flight time
	denAustime.push_back(make_pair(1200, 200));
	denAustime.push_back(make_pair(1500, 220));
	denverFlight[city::Austin] = denAustime;

	vector<pair<int, int>> denChitime; //Denver to Chicago Flight time
	denChitime.push_back(make_pair(700, 220));
	denChitime.push_back(make_pair(800, 250));
	denChitime.push_back(make_pair(1000, 250));
	denChitime.push_back(make_pair(1200, 250));
	denChitime.push_back(make_pair(1400, 250));
	denChitime.push_back(make_pair(1600, 250));
	denChitime.push_back(make_pair(1830, 240));
	denverFlight[city::Chicago] = denChitime;

	flightTable[city::Denver] = denverFlight;





	map<int, vector<pair<int, int>>> chicagoFlight;

	vector<pair<int, int>> chiTortime; //Chicago to Toronto Flight time
	chiTortime.push_back(make_pair(740, 110));
	chiTortime.push_back(make_pair(910, 230));
	chiTortime.push_back(make_pair(1010, 230));
	chiTortime.push_back(make_pair(1110, 230));
	chiTortime.push_back(make_pair(1210, 230));
	chiTortime.push_back(make_pair(1310, 230));
	chiTortime.push_back(make_pair(1410, 230));
	chiTortime.push_back(make_pair(1510, 230));
	chiTortime.push_back(make_pair(1610, 230));
	chiTortime.push_back(make_pair(1710, 230));
	chiTortime.push_back(make_pair(1910, 200));
	chiTortime.push_back(make_pair(2110, 210));
	chicagoFlight[city::Toronto] = chiTortime;

	vector<pair<int, int>> chiAtltime; //Chicago to Atlanta Flight time
	chiAtltime.push_back(make_pair(650, 210));
	chiAtltime.push_back(make_pair(800, 240));
	chiAtltime.push_back(make_pair(900, 240));
	chiAtltime.push_back(make_pair(1000, 240));
	chiAtltime.push_back(make_pair(1100, 240));
	chiAtltime.push_back(make_pair(1200, 240));
	chiAtltime.push_back(make_pair(1300, 240));
	chiAtltime.push_back(make_pair(1400, 240));
	chiAtltime.push_back(make_pair(1500, 240));
	chiAtltime.push_back(make_pair(1600, 240));
	chiAtltime.push_back(make_pair(1700, 240));
	chiAtltime.push_back(make_pair(1800, 240));
	chiAtltime.push_back(make_pair(1900, 240));
	chiAtltime.push_back(make_pair(2000, 240));
	chiAtltime.push_back(make_pair(2150, 300));

	chicagoFlight[city::Atlanta] = chiAtltime;

	vector<pair<int, int>> chiDentime; //Chicago to Denver Flight time
	chiDentime.push_back(make_pair(900, 210));
	chiDentime.push_back(make_pair(1130, 220));
	chiDentime.push_back(make_pair(1330, 220));
	chiDentime.push_back(make_pair(1530, 220));
	chiDentime.push_back(make_pair(1730, 220));
	chiDentime.push_back(make_pair(2100, 250));
	chicagoFlight[city::Denver] = chiDentime;

	vector<pair<int, int>> chiBuftime; //Chicago to Buffalo Flight time
	chiBuftime.push_back(make_pair(1100, 200));
	chiBuftime.push_back(make_pair(1310, 150));
	chiBuftime.push_back(make_pair(1500, 230));
	chiBuftime.push_back(make_pair(1800, 210));
	chicagoFlight[city::Buffalo] = chiBuftime;

	flightTable[city::Chicago] = chicagoFlight;

	map<int, vector<pair<int, int>>> buffaloFlight;

	vector<pair<int, int>> bufChitime; //Buffalo to Chicago Flight time
	bufChitime.push_back(make_pair(940, 140));
	bufChitime.push_back(make_pair(1110, 150));
	bufChitime.push_back(make_pair(1740, 240));
	bufChitime.push_back(make_pair(2010, 220));
	buffaloFlight[city::Chicago] = bufChitime;

	flightTable[city::Buffalo] = buffaloFlight;

	return flightTable;
}

