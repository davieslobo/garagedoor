
class GarageDoor{
   
    
    int doorDistance = 0;
    String doorName ="";
    
    public:
      enum class DoorState {opened, opening, closed, closing, inbetween};
      DoorState doorState = DoorState::inbetween;
      void setDistance(int distance){
        doorDistance = distance;
      }
      int getDistance(){
        return doorDistance;
      }

      void setName(String doorname){
        doorName = doorname;
      }
      String getName(){
        return doorName;
      }
      
      void setState(DoorState doorstate){
       doorState = doorstate;
      }
      
      DoorState getState(){
        return doorState;  
      }
      
      
};


