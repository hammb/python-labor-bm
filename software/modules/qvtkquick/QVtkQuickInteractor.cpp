#include "QVtkQuickInteractor.h"
#include "QVtkQuickRenderWindowItem.h"

#include <QtCore>
#include <QtGui>

#include "vtkCommand.h"
#include "vtkSmartPointer.h"
#include "vtkGenericRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"

static const char* asciiToKeySymTable[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, "Tab", 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    "space", "exclam", "quotedbl", "numbersign",
    "dollar", "percent", "ampersand", "quoteright",
    "parenleft", "parenright", "asterisk", "plus",
    "comma", "minus", "period", "slash",
    "0", "1", "2", "3", "4", "5", "6", "7",
    "8", "9", "colon", "semicolon", "less", "equal", "greater", "question",
    "at", "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "bracketleft",
    "backslash", "bracketright", "asciicircum", "underscore",
    "quoteleft", "a", "b", "c", "d", "e", "f", "g",
    "h", "i", "j", "k", "l", "m", "n", "o",
    "p", "q", "r", "s", "t", "u", "v", "w",
    "x", "y", "z", "braceleft", "bar", "braceright", "asciitilde", "Delete",
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static QMap<Qt::Key, const char*> qtKeyToKeySymTable;
void initializeMappingTable() //Not all compilers support initializer lists (MSVC)
{
    if(!qtKeyToKeySymTable.isEmpty()) return;
    qtKeyToKeySymTable.insert(Qt::Key_Backspace, "BackSpace");
    qtKeyToKeySymTable.insert(Qt::Key_Tab, "Tab");
    qtKeyToKeySymTable.insert(Qt::Key_Backtab, "Tab");
    qtKeyToKeySymTable.insert(Qt::Key_Clear, "Clear");
    qtKeyToKeySymTable.insert(Qt::Key_Return, "Return");
    qtKeyToKeySymTable.insert(Qt::Key_Enter, "Return");
    qtKeyToKeySymTable.insert(Qt::Key_Shift, "Shift_L");
    qtKeyToKeySymTable.insert(Qt::Key_Control, "Control_L");
    qtKeyToKeySymTable.insert(Qt::Key_Alt, "Alt_L");
    qtKeyToKeySymTable.insert(Qt::Key_Pause, "Pause");
    qtKeyToKeySymTable.insert(Qt::Key_CapsLock, "Caps_Lock");
    qtKeyToKeySymTable.insert(Qt::Key_Escape, "Escape");
    qtKeyToKeySymTable.insert(Qt::Key_Space, "space");
    qtKeyToKeySymTable.insert(Qt::Key_PageUp, "Prior");
    qtKeyToKeySymTable.insert(Qt::Key_PageDown, "Next");
    qtKeyToKeySymTable.insert(Qt::Key_End, "End");
    qtKeyToKeySymTable.insert(Qt::Key_Home, "Home");
    qtKeyToKeySymTable.insert(Qt::Key_Left, "Left");
    qtKeyToKeySymTable.insert(Qt::Key_Up, "Up");
    qtKeyToKeySymTable.insert(Qt::Key_Right, "Right");
    qtKeyToKeySymTable.insert(Qt::Key_Down, "Down");
    qtKeyToKeySymTable.insert(Qt::Key_Select, "Select");
    qtKeyToKeySymTable.insert(Qt::Key_Execute, "Execute");
    qtKeyToKeySymTable.insert(Qt::Key_SysReq, "Snapshot");
    qtKeyToKeySymTable.insert(Qt::Key_Insert, "Insert");
    qtKeyToKeySymTable.insert(Qt::Key_Delete, "Delete");
    qtKeyToKeySymTable.insert(Qt::Key_Help, "Help");
    qtKeyToKeySymTable.insert(Qt::Key_0, "0");
    qtKeyToKeySymTable.insert(Qt::Key_1, "1");
    qtKeyToKeySymTable.insert(Qt::Key_2, "2");
    qtKeyToKeySymTable.insert(Qt::Key_3, "3");
    qtKeyToKeySymTable.insert(Qt::Key_4, "4");
    qtKeyToKeySymTable.insert(Qt::Key_5, "5");
    qtKeyToKeySymTable.insert(Qt::Key_6, "6");
    qtKeyToKeySymTable.insert(Qt::Key_7, "7");
    qtKeyToKeySymTable.insert(Qt::Key_8, "8");
    qtKeyToKeySymTable.insert(Qt::Key_9, "9");
    qtKeyToKeySymTable.insert(Qt::Key_A, "a");
    qtKeyToKeySymTable.insert(Qt::Key_B, "b");
    qtKeyToKeySymTable.insert(Qt::Key_C, "c");
    qtKeyToKeySymTable.insert(Qt::Key_D, "d");
    qtKeyToKeySymTable.insert(Qt::Key_E, "e");
    qtKeyToKeySymTable.insert(Qt::Key_F, "f");
    qtKeyToKeySymTable.insert(Qt::Key_G, "g");
    qtKeyToKeySymTable.insert(Qt::Key_H, "h");
    qtKeyToKeySymTable.insert(Qt::Key_I, "i");
    qtKeyToKeySymTable.insert(Qt::Key_J, "h");
    qtKeyToKeySymTable.insert(Qt::Key_K, "k");
    qtKeyToKeySymTable.insert(Qt::Key_L, "l");
    qtKeyToKeySymTable.insert(Qt::Key_M, "m");
    qtKeyToKeySymTable.insert(Qt::Key_N, "n");
    qtKeyToKeySymTable.insert(Qt::Key_O, "o");
    qtKeyToKeySymTable.insert(Qt::Key_P, "p");
    qtKeyToKeySymTable.insert(Qt::Key_Q, "q");
    qtKeyToKeySymTable.insert(Qt::Key_R, "r");
    qtKeyToKeySymTable.insert(Qt::Key_S, "s");
    qtKeyToKeySymTable.insert(Qt::Key_T, "t");
    qtKeyToKeySymTable.insert(Qt::Key_U, "u");
    qtKeyToKeySymTable.insert(Qt::Key_V, "v");
    qtKeyToKeySymTable.insert(Qt::Key_W, "w");
    qtKeyToKeySymTable.insert(Qt::Key_X, "x");
    qtKeyToKeySymTable.insert(Qt::Key_Y, "y");
    qtKeyToKeySymTable.insert(Qt::Key_Z, "z");
    qtKeyToKeySymTable.insert(Qt::Key_Asterisk, "asterisk");
    qtKeyToKeySymTable.insert(Qt::Key_Plus, "plus");
    qtKeyToKeySymTable.insert(Qt::Key_Bar, "bar");
    qtKeyToKeySymTable.insert(Qt::Key_Minus, "minus");
    qtKeyToKeySymTable.insert(Qt::Key_Period, "period");
    qtKeyToKeySymTable.insert(Qt::Key_Slash, "slash");
    qtKeyToKeySymTable.insert(Qt::Key_F1, "F1");
    qtKeyToKeySymTable.insert(Qt::Key_F2, "F2");
    qtKeyToKeySymTable.insert(Qt::Key_F3, "F3");
    qtKeyToKeySymTable.insert(Qt::Key_F4, "F4");
    qtKeyToKeySymTable.insert(Qt::Key_F5, "F5");
    qtKeyToKeySymTable.insert(Qt::Key_F6, "F6");
    qtKeyToKeySymTable.insert(Qt::Key_F7, "F7");
    qtKeyToKeySymTable.insert(Qt::Key_F8, "F8");
    qtKeyToKeySymTable.insert(Qt::Key_F9, "F9");
    qtKeyToKeySymTable.insert(Qt::Key_F10, "F10");
    qtKeyToKeySymTable.insert(Qt::Key_F11, "F11");
    qtKeyToKeySymTable.insert(Qt::Key_F12, "F12");
    qtKeyToKeySymTable.insert(Qt::Key_F13, "F13");
    qtKeyToKeySymTable.insert(Qt::Key_F14, "F14");
    qtKeyToKeySymTable.insert(Qt::Key_F15, "F15");
    qtKeyToKeySymTable.insert(Qt::Key_F16, "F16");
    qtKeyToKeySymTable.insert(Qt::Key_F17, "F17");
    qtKeyToKeySymTable.insert(Qt::Key_F18, "F18");
    qtKeyToKeySymTable.insert(Qt::Key_F19, "F19");
    qtKeyToKeySymTable.insert(Qt::Key_F20, "F20");
    qtKeyToKeySymTable.insert(Qt::Key_F21, "F21");
    qtKeyToKeySymTable.insert(Qt::Key_F22, "F22");
    qtKeyToKeySymTable.insert(Qt::Key_F23, "F23");
    qtKeyToKeySymTable.insert(Qt::Key_F24, "F24");
    qtKeyToKeySymTable.insert(Qt::Key_NumLock, "Num_Lock");
    qtKeyToKeySymTable.insert(Qt::Key_ScrollLock, "Scroll_Lock");
}


class QVtkQuickInteractor::PrivateData
{
public:
    QQuickItem* renderWindowItem;
    vtkSmartPointer<vtkGenericRenderWindowInteractor> interactor;
    QTimer timer;
};

QVtkQuickInteractor::QVtkQuickInteractor(QObject* parent) : QObject(parent)
  ,d_data(new PrivateData)
{
    initializeMappingTable();

    d_data->interactor = vtkSmartPointer<vtkGenericRenderWindowInteractor>::New();
    d_data->interactor->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());
    d_data->interactor->AddObserver(vtkCommand::CreateTimerEvent, this, &QVtkQuickInteractor::vtkTimerEvent);
    d_data->interactor->AddObserver(vtkCommand::DestroyTimerEvent, this, &QVtkQuickInteractor::vtkTimerEvent);
    QObject::connect(&d_data->timer, &QTimer::timeout, [this](){ d_data->interactor->TimerEvent(); });
    d_data->interactor->Initialize();
    d_data->interactor->Start();
    d_data->interactor->Disable();

    d_data->renderWindowItem = qobject_cast<QQuickItem*>(parent);
    if(d_data->renderWindowItem == nullptr) {
        qCritical() << "Parent is not a QQuickItem!";
    }
}

QVtkQuickInteractor::~QVtkQuickInteractor()
{

}

vtkGenericRenderWindowInteractor *QVtkQuickInteractor::getVtkInteractor() const
{
    return d_data->interactor;
}

void QVtkQuickInteractor::setEnabled(bool arg)
{
    if(isEnabled() == arg) return;

    if(arg) {
        d_data->interactor->Enable();
        d_data->interactor->Initialize();
        d_data->interactor->Start();
    }
    else d_data->interactor->Disable();
    emit enabledChanged(arg);
}

bool QVtkQuickInteractor::isEnabled() const
{
    return d_data->interactor->GetEnabled() > 0;
}

void QVtkQuickInteractor::vtkTimerEvent(vtkObject *caller, unsigned long eventId, void *callData)
{
    Q_UNUSED(caller)
    Q_UNUSED(callData)
    if(eventId == vtkCommand::CreateTimerEvent) {
        d_data->timer.start(10);
    }
    else if(eventId == vtkCommand::DestroyTimerEvent) {
        d_data->timer.stop();
    }
}

bool QVtkQuickInteractor::mapEvent(QEvent* e)
{
    if(d_data->interactor == nullptr || e == nullptr)
        return false;

    const QEvent::Type t = e->type();

    if(t == QEvent::Resize)
    {
        QResizeEvent* e2 = static_cast<QResizeEvent*>(e);
        QSize size = e2->size();
        d_data->interactor->SetSize(size.width(), size.height());
        return true;
    }

    if(!d_data->interactor->GetEnabled())
        return false;

    if(t == QEvent::MouseButtonPress ||
            t == QEvent::MouseButtonRelease ||
            t == QEvent::MouseButtonDblClick ||
            t == QEvent::MouseMove)
    {
        QMouseEvent* e2 = static_cast<QMouseEvent*>(e);

        // give interactor the event information
        d_data->interactor->SetEventInformationFlipY(e2->x(), e2->y(),
                                       (e2->modifiers() & Qt::ControlModifier) > 0 ? 1 : 0,
                                       (e2->modifiers() & Qt::ShiftModifier ) > 0 ? 1 : 0,
                                       0,
                                       e2->type() == QEvent::MouseButtonDblClick ? 1 : 0);

        if(t == QEvent::MouseMove) {
            d_data->interactor->InvokeEvent(vtkCommand::MouseMoveEvent, e2);
        }
        else if(t == QEvent::MouseButtonPress || t == QEvent::MouseButtonDblClick) {
            switch(e2->button())
            {
            case Qt::LeftButton:
                d_data->interactor->InvokeEvent(vtkCommand::LeftButtonPressEvent, e2);
                break;

            case Qt::MidButton:
                d_data->interactor->InvokeEvent(vtkCommand::MiddleButtonPressEvent, e2);
                break;

            case Qt::RightButton:
                d_data->interactor->InvokeEvent(vtkCommand::RightButtonPressEvent, e2);
                break;

            default:
                break;
            }
        }
        else if(t == QEvent::MouseButtonRelease) {
            switch(e2->button())
            {
            case Qt::LeftButton:
                d_data->interactor->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, e2);
                break;

            case Qt::MidButton:
                d_data->interactor->InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, e2);
                break;

            case Qt::RightButton:
                d_data->interactor->InvokeEvent(vtkCommand::RightButtonReleaseEvent, e2);
                break;

            default:
                break;
            }
        }
        return true;
    }
    else if(t == QEvent::Enter) {
        d_data->interactor->InvokeEvent(vtkCommand::EnterEvent, e);
        return true;
    }
    else if(t == QEvent::Leave) {
        d_data->interactor->InvokeEvent(vtkCommand::LeaveEvent, e);
        return true;
    }
    else if(t == QEvent::KeyPress || t == QEvent::KeyRelease) {

        QKeyEvent* e2 = static_cast<QKeyEvent*>(e);

        // get key and keysym information
        int ascii_key = e2->text().length() ? e2->text().unicode()->toLatin1() : 0;
        const char* keysym = (ascii_key >= 0) ? asciiToKeySymTable[ascii_key] : nullptr;
        if(!keysym || e2->modifiers() == Qt::KeypadModifier)
            // get virtual keys
            keysym = qtKeyToKeySymTable.value(static_cast<Qt::Key>(e2->key()), "None");

        if(!keysym)
            keysym = "None";

        // give interactor event information
        d_data->interactor->SetKeyEventInformation(
                    (e2->modifiers() & Qt::ControlModifier),
                    (e2->modifiers() & Qt::ShiftModifier),
                    ascii_key, e2->count(), keysym);

        if(t == QEvent::KeyPress) {
            d_data->interactor->InvokeEvent(vtkCommand::KeyPressEvent, e2);

            // invoke char event only for ascii characters
            if(ascii_key)
                d_data->interactor->InvokeEvent(vtkCommand::CharEvent, e2);
        }
        else
            d_data->interactor->InvokeEvent(vtkCommand::KeyReleaseEvent, e2);

        return true;
    }
    else if(t == QEvent::Wheel) {
        QWheelEvent* e2 = static_cast<QWheelEvent*>(e);
        d_data->interactor->SetEventInformationFlipY(e2->x(), e2->y(),
                                       (e2->modifiers() & Qt::ControlModifier) > 0 ? 1 : 0,
                                       (e2->modifiers() & Qt::ShiftModifier ) > 0 ? 1 : 0);

        if(e2->delta() > 0)
            d_data->interactor->InvokeEvent(vtkCommand::MouseWheelForwardEvent, e2);
        else
            d_data->interactor->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, e2);
        return true;
    }

    return false;
}

bool QVtkQuickInteractor::eventFilter(QObject*, QEvent* e)
{
    if(mapEvent(e)) {
        QMetaObject::invokeMethod(d_data->renderWindowItem, "update");
        return true;
    }

    return false;
}
