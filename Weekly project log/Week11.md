# Week 11 Project Log

**Student:** Joe Falkenburg

**Team role:** Secretary

**Teammates:** Ayan Sheikh, Emmett Gillespie, and Daniel Lim

**Project:** Bird-feeder project

**Stakeholder:** Don Fong

**Reporting period:** October 31 through November 6, 2026

## Project work · October 31–November 6

This week we tested, documented, and submitted the MVP, and we kept revising the prototyping assignment. The MVP uses changed-area object detection instead of deer recognition, and more testing showed again that the classifier, our AI approach from prototyping, is not viable. Before submitting, we changed the detector's trigger rule from a fixed changed-area cutoff to one that requires an approaching object.

### New trigger rule · October 31

Under the old rule, a changed region covering at least 5% of the image in three straight frames set off the alarm. With nothing in front of the camera, it kept triggering at about 5.4–5.8%, because small shifts in the reference image and lighting were enough to cross the cutoff. It also treated any large change that stayed in view as a target, whether or not anything was coming closer.

The new rule looks for an approaching object. After the PIR trips, the camera checks up to ten frames. The largest changed region has to be tracked across five frames, grow by at least 25% overall and by at least two percentage points of the image, grow in at least two separate steps, and cover at least 10% of the image in each of the last three frames. A large object that just sits there no longer sets it off, and neither does a single jump in size.

| Indoor test | Largest changed region | Result |
| --- | --- | --- |
| Large object held still | 16.250–16.375% across ten frames | No alarm: large, but not growing |
| Object moved toward the camera | 15.000 → 18.042 → 23.375 → 28.208 → 32.646% | Alarm on the fifth frame, then shut off |
| Empty view right afterward | 0.000% across ten frames | No second alarm |
| Small object in view | 3.792–7.458% across ten frames | No alarm: under the 10% floor even though it grew |

We ran these indoors with the camera fixed, using a narrow object as the large target and a small battery-sized object as the small one. They test the rule itself, not deer recognition or how it does outdoors.

As we agreed last week, the RGB light now lights the reference and detection photos the same way, and it flashes white at 5 Hz through the 5.5-second alarm along with three 2 kHz buzzer pulses. The light uses the display's pins, so we turned the TFT status display off. The PIR only starts a check when no alarm or cooldown is running, and after an alarm the PIR has to go LOW before it can trigger again, so a new detection cannot restart or stretch out an alarm that is already going. That closes the restart protection we still had open last week. If ten frames go by without a confirmation, it waits two minutes before checking again.

### Classifier testing · November 4

We kept testing the classifier, and it is still not a viable solution. Our October 29 test on the board had already shown the problem: the compact deer model ran on the ESP32-S3 in about 380 ms per frame but never confirmed a deer. Over 33 frames of a full-body deer image shown to the camera, its score averaged 0.216 and peaked at 0.378; over 33 frames of a close-up, it averaged 0.083 and peaked at 0.223. The threshold is 0.85. We found the camera and model together fell short because of the image resolution, the lighting, and the latency between components, which is why the MVP uses object detection.

### New speaker · November 6

We got a new speaker to replace the buzzer. We want it to reach the 25–28 kHz range we chose in Week 7, which deer can hear and people cannot, and to be louder than the kit buzzer, which has no published frequency rating. Right now the alarm drives the buzzer at 2 kHz in three 1.5-second pulses with half-second gaps. We confirmed sound and light together under the old trigger rule, but we ran out of jumper wires and unplugged the buzzer before testing the new rule, so those tests show the alarm firing and the light flashing, not the sound.

## MVP demo and submission · November 1–2

On November 1, we met in person at Leutner Pavilion, posed as deer to test the MVP's object detection, and recorded the demo videos for our MVP submission, showing the system triggering when it should and staying off when it should not. We also went over the MVP's shortcomings. The same day, we wrote up the current build: the wiring, a runbook for building, flashing, calibrating, and recording test photos, our test results with photo galleries, a log template for outdoor trials, and these known issues:

| Issue | What it means |
| --- | --- |
| No deer recognition | It reacts to any large object that grows in view. A person can set it off, a small animal close to the camera can look big, and a deer far away can look small. |
| Fixed reference view | Moving the camera, or a change in the scene or lighting, can look like a large change, so the empty view has to be relearned whenever the camera moves. The brightness correction does not remove every shadow or exposure change. |
| Approach assumption | An animal that stands still, walks sideways, backs away, stays far off, or comes in fast can miss the five-frame growth rule within the ten frames. |
| Thresholds from limited tests | We picked the 10% floor and the growth requirements from a small set of indoor tests, and we have no field-accuracy or long-term results. |
| Sound and deterrence | We have not rechecked the sound since unplugging the buzzer, and we have not measured whether the alarm actually drives deer off. |
| Power and outdoor evidence | The whole setup runs on battery, but we have not measured runtime or current draw, and we have no outdoor results yet. |

On November 2, we finished the MVP assignment together and submitted it. It asked us to show the MVP's core operation and what we improved after the midterm, along with a testable requirement, evidence, a description, our next tests, and each member's contributions and planned tasks.

## Revising the prototyping assignment · October 26–November 6

The prototyping assignment was due October 26, and we got the camera module late. A camera was not what Dr. Fu expected us to use to solve the problem, and his early feedback listed it as unavailable. He still ordered one for us, which backed our pursuing that idea, and he understands that waiting on it set us back. That made the assignment hard to put together in time. We scrambled to turn it in on time, and since it was graded right away, the clock on revising it started while we were already behind.

The assignment got a strikingly low score. On Wednesday, October 28, the day after it was graded, I talked with Dr. Fu on behalf of the team and passed his feedback along. Since only the two of us had talked, the team still had questions. Dr. Fu was out of lab that Friday, so despite that first conversation, the whole team could not sync with him until Monday, November 2.

We did the October 30 MVP progress check-in with Roy (TA) because Dr. Fu was out, and Dr. Fu followed up by text. That check-in changed how we saw the prototypes. Our P3 classifier results meant the MVP would use simple object detection instead of inference-based detection, we had to deal with nearby birds and measure false alarms, and each prototype's purpose, test, result, and next step had to be clear. Dr. Fu had also suggested a new dual-sensor setup that would replace the PIR and camera with ultrasonic or other range sensing, and he encouraged us to revise the prototypes around it and pivot the project. We understood him to want it in the prototyping resubmission, since it would sit at the same level of abstraction as P3.

When the whole team met with Dr. Fu on Monday, we showed him our classifier data and videos of the system working and taking pictures. He clarified that we did not need to include the dual-sensor setup, or even pivot. He had pushed the pivot because he thought we had made no progress on P1–P3. We had; our submission just did not include enough evidence of it. It was a misconnection. In that same conversation, Dr. Fu indicated support for our revising the assignment late.

These new understandings came after we turned the assignment in, so in principle a revision could stay with what we had accomplished by the deadline. From past revisions, though, where Dr. Fu clarified this without taking points off, we understood that a revision should be rebased on our updated perspective, pivot included. So we are presenting the prototypes as an iterative path that led to the MVP, not centering an approach that became obsolete or framing the prototyping as forward work on the classifier we dropped.

Dr. Fu was out of lab again on Wednesday, November 4, so we worked on the resubmission without him, revising the P3 checkpoint and reworking P1, P2, and P3 to make each one's purpose, test, result, and next step clear and to include the evidence of progress the first submission lacked.

I also asked Roy whether he had jurisdiction over our late revision, and he said he did, since he grades half of the groups. He supported our case.

At the November 6 verification and validation progress check-in in class, we told Dr. Fu we were continuing to revise the prototyping assignment, and he affirmed it.

## Testing and the November 9 checkpoint · November 2–6

On November 2, we talked about what to test before the next checkpoint, and at our meeting after lab on November 6 we went over future tests. We worked on verification and validation for the Validation Testing Checkpoint on Monday, November 9, where we report the evidence we have and where our stakeholder review stands.

## Enclosure redesign and stakeholder review · November 6

After lab on November 6, around 3 PM, we met in person and went over future tests, a redesign of the enclosure, and asking Mr. Fong to review how it looks. For the redesign, we considered a taller top so the lid can close over the wired breadboard, internal mounts to hold the modules behind their openings, sealing and whether to print in PETG instead of PLA, and how the enclosure looks next to the memorial feeder.

Mr. Fong's feedback has stressed the memorial feeder's appearance, so we want his take on how the enclosure looks and whether it keeps the feeder's inscription visible. We have not emailed him yet.
