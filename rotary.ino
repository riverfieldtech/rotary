/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   rotary.ino                                                       

      PIN2-A相（デジタル入力割り込み）                                    
      PIN3-B相（デジタル入力割り込み）                                     
      PIN4-Z相　通常入力                                                 
      
    Author: riverfield techologies inc.
    Modified: 2022/4/3
	Created Git repository: 2022/4/30
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

byte encoder_a = 2;                     //A相pin2
byte encoder_b = 3;                     //B相pin3
byte encoder_z = 4;                     //Z相pin4
long int  encoder_cnt=0;                     //エンコーダカウント用変数 
int  encoder_rotate_cnt=0;              //エンコーダ回転数カウント用変数

long int  old_encoder_cnt=0;

void setup() {
  pinMode(encoder_a,INPUT_PULLUP);      //A相用信号入力　入力割り込みpinを使用　内蔵プルアップ有効
  pinMode(encoder_b,INPUT_PULLUP);      //B相用信号入力　内蔵プルアップ有効
  pinMode(encoder_z,INPUT_PULLUP);      //Z相用信号入力　内蔵プルアップ有効

  /* デジタル入力割り込みをA相に設定、立ち上がりエッジでencoder_pulse関数を呼び出し  */
  attachInterrupt(digitalPinToInterrupt(encoder_a), encoder_pulse_a, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_b), encoder_pulse_b, CHANGE);
  Serial.begin(9600);
}

void loop() {
  
  Serial.println(encoder_cnt);       //エンコーダカウントをPCに出力   
  
  //Serial.println(encoder_cnt - old_encoder_cnt);
  old_encoder_cnt = encoder_cnt;
  
  delay(50);
}

/*A相の両エッジで割り込み*/
void encoder_pulse_a() {                  
  if(digitalRead(encoder_a)==1){//エッジの後High＝立ち上がりエッジ
    if(digitalRead(encoder_b)==0){//A相が立上がりエッジでB相がLow→CW
      encoder_cnt++;                    //エンコーダカウントをインクリメント
      if(digitalRead(encoder_z)==0){    //Z相がLOWのとき（原点にいるとき）
        encoder_rotate_cnt++;           //回転数をインクリメント
      }
    }else{
      encoder_cnt--;                    //エンコーダカウントをデクリメント
      if(digitalRead(encoder_z)==0){    //Z相がLOWのとき（原点にいるとき）
        encoder_rotate_cnt--;           //回転数をデクリメント
      }
    }
  }else{                      //エッジの後LOW＝立ち下がりエッジ
    if(digitalRead(encoder_b)==1){      //A相が立下りエッジでB相がHigh→CW
      encoder_cnt++;                    //エンコーダカウントをインクリメント
    }else{
      encoder_cnt--;                    //エンコーダカウントをデクリメント
    }
  }
}

/*B相の両エッジで割り込み*/
void encoder_pulse_b() {                  
  if(digitalRead(encoder_b)==1){//エッジの後High＝立ち上がりエッジ
    if(digitalRead(encoder_a)==1){//A相が立上がりエッジでB相がLow→CW
      encoder_cnt++;                    //エンコーダカウントをインクリメント
    }else{
      encoder_cnt--;                    //エンコーダカウントをデクリメント
    }
  }else{                      //エッジの後LOW＝立ち下がりエッジ
    if(digitalRead(encoder_a)==0){      //A相が立下りエッジでB相がHigh→CW
      encoder_cnt++;                    //エンコーダカウントをインクリメント
    }else{
      encoder_cnt--;                    //エンコーダカウントをデクリメント
    }
  }
}
