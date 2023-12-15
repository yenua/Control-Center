//메뉴 선택 시
function showAlert() {
    alert('준비 중입니다!');
}

//세부 메뉴 선택 시 CCTV 핀 버튼 활성화 로직
let activeButton = null;

function toggleButton(buttonId) {
    const button = document.querySelector(`button:nth-child(${buttonId})`);
    //console.log(buttonId);
    if (button.classList.contains('active')) {
        button.classList.remove('active');
        activeButton = null;
        if (buttonId == 1){
            document.getElementById("pin1").style.display = "none";
            document.getElementById("pin2").style.display = "none";
            document.getElementById("pin3").style.display = "none";
            document.getElementById("pin4").style.display = "none";
            document.getElementById("pin5").style.display = "none";
        }
    } else {
        if (activeButton) {
            activeButton.classList.remove('active');
        }
        button.classList.add('active');
        activeButton = button;
        if (buttonId != 1){
            alert('영상 정보 준비 중입니다');
            document.getElementById("pin1").style.display = "none";
            document.getElementById("pin2").style.display = "none";
            document.getElementById("pin3").style.display = "none";
            document.getElementById("pin4").style.display = "none";
            document.getElementById("pin5").style.display = "none";
        }
        else {
            document.getElementById("pin1").style.display = "block";
            document.getElementById("pin2").style.display = "block";
            document.getElementById("pin3").style.display = "block";
            document.getElementById("pin4").style.display = "block";
            document.getElementById("pin5").style.display = "block";
        }
    }
}

// 새 창으로 팝업 열기
function openViewPage(buttonId) {
    var form = document.createElement('form');
    form.method = 'POST';
    form.action = 'view';
    form.target = 'popup'; // 팝업 창의 이름을 설정
    
    var input = document.createElement('input');
    input.type = 'hidden';
    input.name = 'button_id';
    input.value = buttonId;
    
    form.appendChild(input);
    document.body.appendChild(form);
    
    var width = 450;
    var height = 499;
    var left = (window.innerWidth - width) / 2; // 화면 가로 중앙에 위치
    var top = (window.innerHeight - height) / 2; // 화면 세로 중앙에 위치
    var options = 'width=' + width + ',height=' + height + ',left=' + left + ',top=' + top;
    var popup = window.open('', 'popup', options); // 팝업 창 열기
    form.submit();
    document.body.removeChild(form);
}