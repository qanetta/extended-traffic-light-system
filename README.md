# Hệ thống đèn giao thông 2 tuyến đường trên XNUCLEO-F103RB

Bài tập lớn môn **Vi xử lý – Vi điều khiển (CO3009)**
Trường Đại học Bách Khoa – ĐHQG TP.HCM, Khoa Khoa học và Kỹ thuật Máy tính

- Giảng viên hướng dẫn: TS. Lê Trọng Nhân
- Lớp: L02 – Nhóm: 1

## Thành viên

| STT | Họ và tên | MSSV | Nhiệm vụ phụ trách |
|-----|-----------|------|--------------------|
| 1 | Trần Minh Quang | 2412855 | _(cập nhật)_ |
| 2 | Nguyễn Trung Nguyên | 2412363 | _(cập nhật)_ |
| 3 | Nguyễn Hứa Minh Phát | 2412587 | _(cập nhật)_ |

## Giới thiệu

Dự án hiện thực hệ thống đèn giao thông cho ngã tư gồm 2 tuyến đường trên mạch **XNUCLEO-F103RB** (vi điều khiển STM32F103RB). Toàn bộ hệ thống chạy trên một **scheduler O(1)** tự hiện thực, các chế độ hoạt động được điều khiển bằng **máy trạng thái (FSM)**.

## Chức năng

- **Chế độ tự động:** hai tuyến đèn Đỏ – Xanh – Vàng hoạt động luân phiên theo thời gian cài đặt.
- **Chế độ bằng tay:** chuyển trạng thái đèn bằng nút nhấn.
- **Chế độ chỉnh thời gian:** thay đổi thời gian đèn Đỏ – Xanh – Vàng bằng nút nhấn, thông tin hiển thị trên LCD.
- **Hiển thị môi trường:** đọc cảm biến **DHT20**, hiển thị nhiệt độ và độ ẩm trên LCD (giao tiếp I2C).
- **Scheduler O(1):** `SCH_Add_Task`, `SCH_Delete_Task`, `SCH_Update`, `SCH_Dispatch_Tasks`; hỗ trợ one-shot task.

## Phần cứng

| Thành phần | Mô tả |
|------------|-------|
| Board | XNUCLEO-F103RB (STM32F103RB) |
| Cảm biến | DHT20 (nhiệt độ, độ ẩm) |
| Hiển thị | LCD giao tiếp I2C |
| Đèn giao thông | 2 tuyến, mỗi tuyến Đỏ – Xanh – Vàng |
| Nút nhấn | Chuyển chế độ, tăng/giảm, xác nhận |

> Sơ đồ nối chân xem trong báo cáo (mục 3.1 Kết nối phần cứng).

## Cấu trúc thư mục

> Cập nhật lại cho khớp với dự án thực tế.

```
.
├── Core/
│   ├── Inc/            # File header (.h)
│   └── Src/            # File mã nguồn (.c)
├── Drivers/            # HAL / CMSIS do STM32CubeMX sinh ra
├── <ten_du_an>.ioc     # Cấu hình STM32CubeMX
├── .gitignore
└── README.md
```

Các module chính (dự kiến): `scheduler`, `software_timer`, `button`, `traffic_light`, `dht20`, `lcd_i2c`, `fsm_automatic`, `fsm_manual`, `fsm_tuning`.

## Build và nạp chương trình

1. Cài **STM32CubeIDE**.
2. Clone repo:
   ```bash
   git clone https://github.com/<tai-khoan>/<ten-repo>.git
   cd <ten-repo>
   git checkout dev
   ```
3. Mở STM32CubeIDE → **File → Open Projects from File System…** → chọn thư mục repo.
4. **Project → Build Project** (Ctrl + B).
5. Cắm board qua cổng USB (ST-LINK), bấm **Run** để nạp chương trình.

## Hướng dẫn vận hành

_(Cập nhật: chức năng từng nút nhấn, cách chuyển giữa các chế độ, ý nghĩa nội dung hiển thị trên LCD.)_

## Quy ước làm việc nhóm

**Nhánh**

| Nhánh | Vai trò |
|-------|---------|
| `main` | Bản ổn định, dùng để nộp bài |
| `dev` | Gom và kiểm thử các tính năng đã hoàn thành |
| `feature/<ten>` | Mỗi phần việc một nhánh, tách từ `dev` |

Quy trình: `feature/*` → Pull Request → `dev` → (ổn định) → `main`.

**Đặt tên**

- File: chữ thường, nối bằng gạch dưới, ví dụ `fsm_automatic.c`.
- Hàm: `<module>_<hành_động>`, ví dụ `button_read()`, `traffic_set_state()`.

**Thông điệp commit** (Conventional Commits): `<loại>(<phạm vi>): <mô tả ngắn>`

| Loại | Dùng khi |
|------|----------|
| `feat` | Thêm chức năng mới |
| `fix` | Sửa lỗi |
| `refactor` | Tái cấu trúc, không đổi hành vi |
| `test` | Kiểm thử |
| `docs` | Tài liệu |
| `chore` | Cấu hình, thiết lập chung |

Ví dụ: `feat(scheduler): add one-shot task support`

Mỗi thành viên commit bằng tài khoản GitHub của chính mình.

## Tài liệu

- Báo cáo bài tập lớn: _(thêm link hoặc đường dẫn khi hoàn thành)_
