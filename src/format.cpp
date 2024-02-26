#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hoursNum, minutesNum, secondsNum;
  string hoursNumStr, minutesNumStr, secondsNumStr;
  string m_hours, m_minutes, m_seconds;

  hoursNum   = int(seconds/3600);
  minutesNum = int(((seconds / 3600.) - hoursNum)  * 60);
  secondsNum = int(((((seconds / 3600.) - hoursNum) * 60 ) - minutesNum) * 60);

  hoursNumStr = std::to_string(hoursNum);
  minutesNumStr = std::to_string(minutesNum);
  secondsNumStr = std::to_string(secondsNum);
  
  m_hours = (hoursNumStr.length() < 2)? "0" + hoursNumStr : hoursNumStr;
  m_minutes = (minutesNumStr.length() < 2)? "0" + minutesNumStr : minutesNumStr;
  m_seconds = (secondsNumStr.length() < 2)? "0" + secondsNumStr : secondsNumStr;
  
  return m_hours + ":" + m_minutes + ":" + m_seconds;
}