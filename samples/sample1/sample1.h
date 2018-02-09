#include <widgettoolkit/widgettoolkit.h>

class Demo : public akApplication, akActionReceiver {
public:
    Demo(const string& name);
    virtual void ActionPerformed(akControl* sender);

private:
    akWindow* mWnd;
    akTextBox* mTextBox1;
    akListBox* mListBox1;
    akPushButton* mButton1;
};
