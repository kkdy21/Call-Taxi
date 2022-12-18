# 프로젝트명
택시 프로그램 (기사용, 손님용)
# 프로젝트기간
2021.09.07. ~ 2021.09.13
# 개발인원
고대연, 서근원
# 개발환경
- Visual Studio 2019 / C# Winform / Javascript
- Linux Ubuntu 20.04 / C++ 9.3.0 / Visual Studio Code 1.57.1 / MySQL
# 프로그램 소개
Tmap openAPI를 활용한 손님용, 기사용 택시 프로그램
# 주요기능
1. 손님 클라이언트
- 장소검색, 검색한 장소 지도에 핀 띄우기 , 출발지 도착지 설정후 최단 경로 보기, 택시호출, 코멘트 남기기
2. 기사 클라이언트
- 기사가 운행한 정보 열람, 코멘트 열람, 호출 승낙, 기사들간 동기화
# 느낀점
프로그램을 만들다보면 가공된 데이터, 구현되어진 기능을 가져다가 쓸일이 많은데 이번 프로젝트에서 javascript로 구현된 기능을 쓸수 있어서 좋았다. 
c#에 크롬을 띄울때 오류없이 쓰려면 ChfSharp를 사용해야 된다는 점, javascript와 c#을 연동하는 방법이 인상깊었다.
openAPi는 javascript형태로 제공되는것 뿐만아니라 Json, Xml형태로도 제공이 되는데 다음에 api를 사용할 일이 생긴다면 이 두개를 활용해 보고 싶다.

# 구현 이미지
### 손님 클라이언트
<img width="478" alt="image" src="https://user-images.githubusercontent.com/86215246/208296167-b881dfde-2d27-4eff-bbe0-19d558a27adb.png">
<img width="478" alt="image" src="https://user-images.githubusercontent.com/86215246/208296176-4b4fcc87-d773-4738-b007-cc3bc96e9d6e.png">
<img width="478" alt="image" src="https://user-images.githubusercontent.com/86215246/208296181-e8eee8b7-8211-4416-bc0b-cc72424efcb3.png">
<img width="478" alt="image" src="https://user-images.githubusercontent.com/86215246/208296207-ad210eb0-60a2-4728-9184-b9c62bf604d6.png">
<img width="478" alt="image" src="https://user-images.githubusercontent.com/86215246/208296215-a2fe4a9f-a1cd-4b77-8d08-2f4dcd464bea.png">

### 기사 클라이언트
<img width="476" alt="image" src="https://user-images.githubusercontent.com/86215246/208296233-23db6b4c-6cc0-4854-8bbd-cd6061028bec.png">
<img width="479" alt="image" src="https://user-images.githubusercontent.com/86215246/208296239-de72b7b6-8646-48a8-af41-279198863f33.png">
<img width="479" alt="image" src="https://user-images.githubusercontent.com/86215246/208296261-12806729-3fbe-4502-a7a4-c10dff534f57.png">
<img width="474" alt="image" src="https://user-images.githubusercontent.com/86215246/208296269-2cabbabf-da64-4aab-ac72-56e568c8281b.png">
<img width="474" alt="image" src="https://user-images.githubusercontent.com/86215246/208296277-673f8546-0169-4348-83eb-bc501d34a4d3.png">
<img width="474" alt="image" src="https://user-images.githubusercontent.com/86215246/208296285-628bf946-f483-4151-9a47-295893a54c6e.png">

### 순서도
![taxiFlowChart](https://user-images.githubusercontent.com/86215246/208296637-6ae7ec93-6443-433f-9a73-5493a5db7a2c.png)

