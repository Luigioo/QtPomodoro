# QtPomodoro

QtPomodoro 是一个简洁而又功能强大的番茄工作法计时器，使用 Qt 开发。用户可以使用它来集中精力、提高效率，同时也可以在休息时候欣赏音乐。

## 主要功能

- **透明窗口和拖动功能**：QtPomodoro 具有透明的窗口和阴影效果，同时支持窗口拖动。
  
- **自定义工作时长**：用户可以通过右键菜单自定义工作时长，范围从 5 分钟到 45 分钟。
  
- **音乐播放**：工作时长结束时，QtPomodoro 会自动播放随机音乐，增加工作乐趣。
  
- **交互式提示音**：进行某些操作如点击或鼠标悬停时，会播放提示音，增加交互体验。

## 如何使用

1. **启动/停止计时器**：点击 play 按钮开始或停止计时器。
  
2. **自定义工作时长**：右键点击主窗口，选择“时长”从上下文菜单中设置你的工作时间。
  
3. **音乐播放器**：右键点击主窗口，选择“音乐”从上下文菜单中打开音乐播放器，你可以调整音量或切换下一首歌曲。

## 开发和扩展

QtPomodoro 的源代码结构清晰，开发者可以轻松地为其添加新功能或进行修改。

- **音乐扩展**：你可以在 `music` 文件夹中添加你喜欢的音乐，QtPomodoro 会自动识别并播放。

- **样式定制**：QtPomodoro 使用 Qt StyleSheet 来设定应用程序的样式，你可以根据自己的喜好进行修改。

## 依赖和编译

确保你的开发环境已经安装了 Qt 库和 Qt Creator。打开项目文件并使用 Qt Creator 进行编译。

---

希望 QtPomodoro 能助你提高工作效率，享受工作和休息的时光！


# QtPomodoro

QtPomodoro is a Pomodoro timer developed using the Qt framework. It provides a mechanism for users to manage their work sessions, interspersed with short breaks.

## Features

- **Transparent Window with Drag Functionality**: QtPomodoro has a transparent window with a shadow effect, and it supports window dragging.

- **Customizable Work Duration**: Users can set their desired work duration, ranging from 5 to 45 minutes, through a context menu.

- **Music Playback**: At the end of a work session, QtPomodoro can play random music tracks.

- **Interactive Audio Feedback**: Certain interactions, like clicks or hover events, trigger auditory feedback to enhance user interaction.

## Usage

1. **Start/Stop Timer**: Click the play button to initiate or terminate the timer.

2. **Set Work Duration**: Right-click on the main window and select the "Duration" option from the context menu to define your work duration.

3. **Music Player**: Right-click on the main window and select the "Music" option from the context menu to open the music player, where you can adjust the volume or switch to the next track.

## Development and Extension

The source code of QtPomodoro is structured for clarity, making it easy for developers to add new features or modifications.

- **Music Extension**: Music files can be added to the `music` directory, and QtPomodoro will automatically recognize and play them.

- **Styling**: QtPomodoro employs Qt StyleSheet for theming, which developers can adjust according to preferences.

## Dependencies and Compilation

Ensure that the Qt libraries and Qt Creator are installed in your development environment. Open the project file and compile using Qt Creator.
```
