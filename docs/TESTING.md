# Test checklist

ใช้หลัง `make` ใน Linux/WSL

| กรณี | คำสั่ง | ผลที่คาดหวัง |
| --- | --- | --- |
| Help | `help` | แสดงรายการคำสั่ง |
| คำสั่งว่าง | กด Enter | แสดง prompt ใหม่ |
| ไม่มี program | `run` | แจ้งรูปแบบคำสั่ง |
| โปรแกรมและ argument | `run echo hello` | Child แสดง PID/PPID แล้วพิมพ์ `hello`; Parent รายงานสถานะ 0 |
| Working directory | `run pwd` | แสดง working directory จาก Child |
| โปรแกรมไม่มีอยู่ | `run no-such-program` | รายงาน `exec failed`; Parent บันทึกสถานะ 127 |
| Synchronization | `run sleep 1` | prompt ถัดไปปรากฏหลังครบประมาณ 1 วินาที |
| History | `history` | แสดง PID, PPID, ผลลัพธ์ และคำสั่งทุก `run` |
| Exit | `exit` | โปรแกรมแสดง Goodbye และจบการทำงาน |
