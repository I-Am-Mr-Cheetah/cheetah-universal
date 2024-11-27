### Cheetah Universal

## NOTE
Cheetah Universal is in a very early, non-functional state. Most functionality is not expected to be provided and the following following description is [borrowed from the original Cheetah app](https://github.com/leetcode-mafia/cheetah) for description of desired functionalities. Functionality, scope, usability and capabilities may change at any time.

# Cheetah

Cheetah Universal is a crossplatform port of the original [cheetah MacOS application](https://github.com/leetcode-mafia/cheetah), an AI-powered app designed to assist users during remote software engineering interviews by providing real-time, discreet coaching and live coding platform integration.

With Cheetah Universal, you can improve your interview performance and increase your chances of landing that $300k SWE job, without spending your weekends cramming leetcode challenges and memorizing algorithms you'll never use.

## How it works

Cheetah Universal leverages Whisper for real-time audio transcription and GPT-4 for generating hints and solutions. You need to have your own OpenAI API key to use the app. If you don't have access to GPT-4, gpt-3.5-turbo may be used as an alternative.

Whisper runs locally on your system, utilizing Georgi Gerganov's [whisper.cpp](https://github.com/ggerganov/whisper.cpp). An AMD APU, Mac M1 or M2 processor, or discrete GPU is required for optimal performance.

## Getting started

### Prerequisites

Requires macOS 13.1 or later, Windows 10, 11 or later, or a system that supports C++ 20 language features.

To build Cheetah, one must ensure Qt 6.5 or later is available on your system, and CMake 3.18 or later is installed.
Use QtCreator or a typical CMake build process:

`mkdir build && cd build && cmake .. && make`

### Audio driver setup

For the best results, ensure the audio input captures both sides of the conversation.

When using a video chat app like Zoom or Google Meet, you can achieve this with [BlackHole](https://existential.audio/blackhole/), on MacOS a free audio loopback driver. Follow the instructions for setting up a [Multi-Output Device](https://github.com/ExistentialAudio/BlackHole/wiki/Multi-Output-Device), and remember not to use the loopback device as input for the video chat app.

For Linux, Pipewire or PulseAudio already supplies a loopback driver.

### App overview

Open the app and select an audio input to start live transcription. A snippet of the transcription will be displayed under the audio input selector.

*Note:* running the app in debug mode will result in very slow audio transcription performance.

The UI features three buttons:

**Answer:** Generates an answer for the interviewer's question.

**Refine:** Updates the existing answer, useful for when the interviewer provides additional constraints or clarification.

**Analyze:** Analyzes code and logs from the live coding environment in your web browser. Requires the browser extension.

You can also select (highlight) a portion of a generated answer and click Refine to get more detail.

### Installing the browser extension

This is currently not supported.

## Disclaimer

Use of Cheetah Universal violates most company interviewing policies. It may generate incorrect or inappropriate solutions. Users should exercise caution and take responsibility for the information provided by the app.
