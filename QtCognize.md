qApp->setStyleSheet(styledata);
->存储在qApp的QApplicationPrivate::styleSheet变量中，QString，没有立即应用，其他地方通过QString QApplication::styleSheet() const;获取
->void QWidgetPrivate::setStyle_helper(QStyle *newStyle, bool propagate)中先看widget自己有没有QSS，这个优先级最高，再看app或者父级的
【WA_SetStyle 的作用是 告知 Qt 这个控件自己已经设置了 style（或者希望强制使用特定 style），而不是继承父控件或 QApplication 的全局 style。】会先看这个标志
如果没有设置就看parentStyle
->void QWidgetPrivate::setStyle_helper(QStyle *newStyle, bool propagate);函数中传入了parentStyle

QFrame是可以正确地拿到除去外边距，边框，内边距的部分，通过contentsRect
QWidget的contentsRect返回的跟rect()没差别


contentsRect的x,y是相较于自身坐标系的，自身坐标系就是自身可绘制的区域，包含外边距，边框，内边距，内容区域


