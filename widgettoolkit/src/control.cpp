#include <widgettoolkit/control.h>

akControl::akControl(akRect rect, akView* parent /*= NULL*/)
    : akView(rect, parent)
{
}

void akControl::AddActionReceiver(akActionReceiver* receiver)
{
    mActionReceivers.push_back(receiver);
}

void akControl::RemoveActionReceiver(akActionReceiver* receiver)
{
    for (unsigned int i = 0; i < mActionReceivers.size(); i++) {
        akActionReceiver* receiverToDelete = mActionReceivers.at(i);
        if (receiverToDelete == receiver) {
            mActionReceivers.erase(mActionReceivers.begin() + i);
            return;
        }
    }
}

void akControl::InvokeActionReceivers(akControl* sender)
{
    for (unsigned int i = 0; i < mActionReceivers.size(); i++) {
        akActionReceiver* rcv = mActionReceivers.at(i);
        rcv->ActionPerformed(sender);
    }
}
