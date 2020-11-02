### Bài tập tuần môn Iot và ứng dụng
### xem yêu cầu chi tiết của từng bài tập tuần tại [đây](./iot.pptx)
### lưu ý
1) Lỗi can not open /dev/ttyUSB0 trên ubuntu
###### chạy lần lượt các câu lệnh sau
	sudo su
	cd /dev
	chown username ttyUSB0
[link](https://github.com/esp8266/source-code-examples/issues/26) <br>
2) Lỗi Serial.print() không hoạt động trên ubuntu
- trước khi print nên đặt delay(1000) và chỉnh monitor_speed=9600
### tài liệu tham khảo
- [ArduinoMQTT](https://github.com/monstrenyatko/ArduinoMqtt)
- https://www.tinkercad.com/learn/circuits
- https://www.hivemq.com/mqtt-client-library-encyclopedia/
- https://www.rabbitmq.com/#getstarted
- https://learn.adafruit.com/adafruit-arduino-lesson-2-leds/blinking-the-led
- https://genk.vn/huong-dan-lam-cong-tac-thong-minh-bat-tat-bang-smartphone-thong-qua-arduino-sieu-don-gian-20161210152024153.chn
- http://arduino.vn/result/1449-dieu-khien-bat-tat-den-led-tu-xa-thong-qua-nodejs-va-websocket-voi-thu-vien-socketio
