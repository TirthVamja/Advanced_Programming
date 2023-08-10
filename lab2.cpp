#include <bits/stdc++.h>
using namespace std;

class Date {
    
    public:
    int day,month,year;
    string dayOfWeek;
    
    Date(){
        
    }
    
    Date(int dt, int mth, int yr){
        day=dt;
        month=mth;
        year=yr;
        dayOfWeek = getWeekday(day, month, year);
    }
    
    string getWeekday(int day, int month, int year) {
        if (month < 3) {
            month += 12;
            year--;
        }
    
        int k = year % 100;
        int j = year / 100;
        int h = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    
        string Weekday[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
        return Weekday[(h + 5) % 7];
    }
    
    string printDate(){
        string mth[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
        string dateString="";
        dateString = to_string(day)+" "+mth[month-1]+" "+to_string(year)+" ,"+dayOfWeek;
        return dateString;
    }
    
};


struct Notes {
    Date date;
    string content;
};


class Calendar {
    
    Date dt;
    vector<Notes> NotesList;
    vector<int> Holiday;    
    
    public:
    Calendar(){
        
    }
    Calendar(Date d){
        dt=d;
    }
    
    
    string getMonth(){
        string mth[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
        return mth[dt.month-1];
    }
    string getYear(){
        return to_string(dt.year);
    }
    string getDay(){
        return dt.dayOfWeek;
    }
    
    Date getNextWeekday(){
        string day = dt.dayOfWeek;
        int daysToAdd = 1;
        if(day=="Friday") daysToAdd+=2;
        else if(day=="Saturday") daysToAdd+=1;
        
        return addDaysToDate(daysToAdd);
    }
    
    bool isPreceded(Date d1, Date d2){
        int weighted_d1 = 10000*d1.year + 100*d1.month + d1.day;
        int weighted_d2 = 10000*d2.year + 100*d2.month + d2.day;
        return (weighted_d1<weighted_d2)?true:false;
    }
    
    
    Date getNextHoliday(){
        string day = dt.dayOfWeek;
        int daysToAdd = 0;
        if(day=="Monday") daysToAdd += 5;
        else if(day=="Tuesday") daysToAdd += 4;
        else if(day=="Wednesday") daysToAdd += 3;
        else if(day=="Thursday") daysToAdd += 2;
        else if(day=="Friday") daysToAdd += 1;
        else if(day=="Saturday") daysToAdd += 1;
        else daysToAdd += 6;
        
        return addDaysToDate(daysToAdd);
    }
    
    Date addDaysToDate(int daysToAdd){
        int daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30};
        
        int nextWeekDayDate=dt.day, nextWeekDayMonth=dt.month, nextWeekDayYear=dt.year;
        if(daysInMonth[dt.month]-(dt.day+daysToAdd)<0){
            nextWeekDayDate = (dt.day+daysToAdd)%daysInMonth[dt.month-1];
            if(dt.month==12){
                nextWeekDayMonth=1;
                nextWeekDayYear = dt.year+1;
            }
            else nextWeekDayMonth = dt.month+1;
        }
        else nextWeekDayDate+=daysToAdd;
        
        // cout<<nextWeekDayDate<<" "<<nextWeekDayMonth<<" "<<nextWeekDayYear<<endl;
        
        return Date(nextWeekDayDate, nextWeekDayMonth, nextWeekDayYear);
    }
    
    Date iterateDates(Date dt, int daysToAdd) {
        int year=dt.year, month=dt.month, day=dt.day;
        while (daysToAdd > 0) {
            int daysInMonth;
            if (month == 2)
                daysInMonth = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28;
            else if (month == 4 || month == 6 || month == 9 || month == 11)
                daysInMonth = 30;
            else daysInMonth = 31;
    
            int daysRemainingInMonth = daysInMonth - day + 1;
    
            if (daysRemainingInMonth <= daysToAdd){
                daysToAdd -= daysRemainingInMonth;
                day = 1;
                if (month == 12) {
                    month = 1;
                    year++;
                }
                else month++;
            } 
            else{
                day += daysToAdd;
                break;
            }
        }
        return Date(day,month,year);
    }
    
    
    void addNoteToDate(Date d, string note) {
        Notes notes_obj = {
            d,
            note
        };
        NotesList.push_back(notes_obj);
    }
    
    bool isEqual(Date d1, Date d2){
        return (!isPreceded(d1, d2) && !isPreceded(d2, d1));
    }
    
    bool removeNoteFromDate(Date d) {
        vector < Notes > ::iterator referenceForDelete = NotesList.end();
        bool deleted = false;
        for (auto it = NotesList.begin(); it != NotesList.end(); it++) {
            if (isEqual(( * it).date, d)) {
                NotesList.erase(it);
                deleted = true;
                break;
            }
        }
        return deleted;
    }
    
    
    string getNote(Date d){
        // Iterate over NotesList and return string content if it exists...
        for (auto it = NotesList.begin(); it != NotesList.end(); it++) {
            if (isEqual(( * it).date,d)) return (*it).content;
        }
        return "";
    }
    
    
    vector < Notes > getNotesFromDateTillN(Date d, int n) {
        vector < Notes > notesFromDateTillN;
        while (n--) {
            string content = getNote(d);
            if (content != "") {
                Notes noteObj = {
                    d,
                    content
                };
                notesFromDateTillN.push_back(noteObj);
            }
            d = iterateDates(d, 1);
        }
        return notesFromDateTillN;
    }
    
    
    int getDistanceBetweenDates(Date d1, Date d2) {
        if(!isPreceded(d1,d2)){
            Date d = d1;
            d1 = d2;
            d2 = d;
        }
        int difference = 0;
        while (!isEqual(d1,d2)){
            difference++;
            d1 = iterateDates(d1, 1);
        }
        return difference;
    }
    
    
    void printMonthOfYear(int month, int year){
        int daysInMonth;
        if (month == 2)
            daysInMonth = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28;
        else if (month == 4 || month == 6 || month == 9 || month == 11)
            daysInMonth = 30;
        else daysInMonth = 31;
        
        string firstDayOfWeek = dt.getWeekday(1, month, year);
        cout<<firstDayOfWeek<<endl;
        string Weekday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        int wd=0;
        cout<<"SU MO TU WE TH FR SA"<<endl;
        while(Weekday[wd++]!=firstDayOfWeek) cout<<"-  ";
        for(int i=1;i<=daysInMonth;i++){
            (i>10)?cout<<i<<" ":cout<<i<<"  ";
            if(wd==0) cout<<endl;
            wd=(wd+1)%7;
        }
    }
    
    
    int maskDate(Date d){
        return d.year*10000 + d.month*100 + d.day;
    }
    
    
    void addHoliday(Date d){
        Holiday.push_back(maskDate(d));
    }
    
    
    int getNumberOfHolidays(int month, int year){
        int mask = year*10000+month*100, numberOfHolidays=0;
        for(auto& it: Holiday){
            if(it-mask>=0 && it-mask<31) numberOfHolidays++;
        }
        return numberOfHolidays;
    }
    
    
    Date getNextHoliday(Date d){
        for(int i=1;i<=6;i++){
            Date testDate = iterateDates(d,i);
            string weekDay = dt.getWeekday(testDate.day, testDate.month, testDate.year);
            if(weekDay == "Saturday" || weekDay == "Sunday") return testDate;
            for(auto& it: Holiday){
                if(maskDate(testDate) == it) return testDate;
            }
        }
        return d; // code will never reach here
    }
    
    
    Date getNextWeekDay(Date d){
        Date nextDate = iterateDates(d,1);
        Date nextHoliday = getNextHoliday(d);
        while(isEqual(nextDate, nextHoliday)){
            d=nextDate;
            nextDate = iterateDates(d,1);
            nextHoliday = getNextHoliday(d);
        }
        return nextDate;
    }
    
    
};

int main() {
    
    Date d(26,12,2022);
    Calendar c(d);
    
    cout<<d.printDate()<<endl;
    cout<<c.getMonth()<<endl;
    cout<<c.getYear()<<endl;
    cout<<c.getDay()<<endl;
    
    Date new_date = c.iterateDates(d,4); //Remianing for negative integers...
    cout<<new_date.printDate()<<endl;
    
    cout<<c.isPreceded(d,new_date)<<endl;
    c.addNoteToDate(new_date,"Added note by Tirth.");
    c.addNoteToDate(c.iterateDates(new_date,2),"Added note by Vasu.");
    cout<<c.removeNoteFromDate(d)<<endl;
    // cout<<c.removeNoteFromDate(d)<<endl;
    for(auto& i: c.getNotesFromDateTillN(d,10)){
        cout<<i.date.printDate()<<" --> "<<i.content<<endl;
    }
    

    cout<<c.getDistanceBetweenDates(d,new_date)<<endl;
    
    c.printMonthOfYear(12, 2022);
    
    
    c.addHoliday(d);
    // c.addHoliday(c.iterateDates(d,1));
    c.addHoliday(new_date);
    cout<<c.getNumberOfHolidays(12,2022)<<endl;
    
    cout<<c.getNextHoliday(d).printDate()<<endl;
    
    cout<<c.getNextWeekDay(d).printDate()<<endl;
    
    

    
}