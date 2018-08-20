
class GarageDoor{
    const int DOOR_CLOSED_DISTANCE = 10; 
    const int DOOR_OPEN_DISTANCE = 5;
    
    
    int doorDistance = 0;
    char doorName;
    
    public:
      enum class DoorState {open, opening, closed, closing, inbetween};
      DoorState doorState = DoorState::inbetween;
      void setDistance(int distance){
        doorDistance = distance;
      }
      int getDistance(){
        return doorDistance;
      }

      void setName(char name){
        doorName = name;
      }
      char getName(){
        return doorName;
      }
      
      void setState(DoorState doorstate){
       doorState = doorstate;
      }
      
      DoorState getState(){
        return doorState;  
      }
      
      
};


