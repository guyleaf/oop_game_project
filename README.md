# OOP 物件導向程式設計實習 - 遊戲實作 電眼美女

OOP 物件導向程式設計課程分為兩學期
- 第一學期: 訓練基礎 C++ 與 OO 理論概念
- 第二學期: 實作模仿任一款 2D 遊戲，並以 OO 開發

### 成果報告: https://guyleaf.pse.is/3925m4

### 安裝需求
- Visual studio 2017，選擇 Desktop development with C++
- Astyle extension: https://marketplace.visualstudio.com/items?itemName=Lukamicoder.AStyleExtension

### 如何編譯
1. 開啟 solution file: game.sln
2. 選擇 Release x86
3. 執行編譯 (綠色箭頭)

### 資料夾介紹
- Misc: 雜項，包含 UML、Report 使用的圖片
- RES: 遊戲內的所有圖片素材，依照 角色 Class 分門別類，根目錄為遊戲本體圖片素材
- Sounds: 遊戲內的所有音效與音樂
- Source: 所有遊戲程式碼

### 整體架構:
Start -> OnInit(only once) -> CGameStateInit -> CGameStateRun -> CGameStateOver  
-> CGameStateInit (loop)... (until Alt + F4)-> Stop
