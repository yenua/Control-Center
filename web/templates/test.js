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

// 핀 버튼 클릭 이벤트 핸들러 등록
document.getElementById("pin1").addEventListener("click", function() {
    openPopup("새 탭 팝업", "https://example.com");
});
document.getElementById("pin2").addEventListener("click", function() {
    openPopup("새 탭 팝업", "https://example.com");
});
document.getElementById("pin3").addEventListener("click", function() {
    openPopup("새 탭 팝업", "https://example.com");
});
document.getElementById("pin4").addEventListener("click", function() {
    openPopup("새 탭 팝업", "https://example.com");
});
document.getElementById("pin5").addEventListener("click", function() {
    openPopup("새 탭 팝업", "https://example.com");
});

// 팝업 창 열기
function openViewPage(buttonId) {
    var form = document.createElement('form');
    form.method = 'POST';
    form.action = '/view';
    form.target = '_blank'; // 새 탭에서 열릴 수 있도록 target 속성을 설정
    
    var input = document.createElement('input');
    input.type = 'hidden';
    input.name = 'button_id';
    input.value = buttonId;
    
    form.appendChild(input);
    document.body.appendChild(form);
    
    form.submit();
    document.body.removeChild(form);
}