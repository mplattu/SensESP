#ifdef HADDOCK_SENSOR_LEVEL_BILGE_BEEF

SKMetadata* metadataLevelBilge = new SKMetadata();
metadataLevelBilge->units_ = "mm";
metadataLevelBilge->description_ = "Bilge Water Level";
metadataLevelBilge->display_name_ = "Bilge Level";
metadataLevelBilge->short_name_ = "Bilge Level";

auto* pwm_bilge = new PwmDistanceDypa02yy(A0, 1000, 340, "/Bilge/Measurement");
pwm_bilge
  ->connect_to(new MovingAverage(1000, 1, "/Bilge/MovingAverage"))
  ->connect_to(new Linear(1.8, 0, "/Bilge/LinearTransform"))
  ->connect_to(new SKOutputNumber("bilge.0.currentLevel", "", metadataLevelBilge));

#endif
