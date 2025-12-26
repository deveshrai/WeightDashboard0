#include "SaveFileSpecialDialog.h"

#if defined(Q_OS_IOS)

#import <UIKit/UIKit.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

@interface SaveFilePickerDelegate : NSObject <UIDocumentPickerDelegate>
@property (nonatomic, strong) NSURL *selectedUrl;
@property (nonatomic, assign) BOOL finished;
@end

@implementation SaveFilePickerDelegate
- (void)documentPicker:(UIDocumentPickerViewController *)controller
 didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls
{
    self.selectedUrl = urls.firstObject;
    self.finished = YES;
}

- (void)documentPickerWasCancelled:(UIDocumentPickerViewController *)controller
{
    self.finished = YES;
}
@end

QString SaveFileSpecialDialog::saveFile(const QByteArray &data, const QString &suggestedName)
{
    // Convert QString → NSString
    NSString *fileName = suggestedName.toNSString();

    // Create temp path
    NSString *tempPath = [NSTemporaryDirectory() stringByAppendingPathComponent:fileName];
    NSURL *tempUrl = [NSURL fileURLWithPath:tempPath];

    // Write actual SQLite bytes to temp file
    NSData *nsData = [NSData dataWithBytes:data.data() length:data.size()];
    [nsData writeToURL:tempUrl atomically:YES];

    // Create picker in export mode
    UIDocumentPickerViewController *picker =
        [[UIDocumentPickerViewController alloc]
            initWithURLs:@[tempUrl]
            inMode:UIDocumentPickerModeExportToService];

    SaveFilePickerDelegate *delegate = [SaveFilePickerDelegate new];
    picker.delegate = delegate;

    // Present modally
    UIViewController *root = UIApplication.sharedApplication.keyWindow.rootViewController;
    [root presentViewController:picker animated:YES completion:nil];

    // Block until user finishes
    while (!delegate.finished) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:[NSDate dateWithTimeIntervalSinceNow:0.1]];
    }

    if (!delegate.selectedUrl)
        return QString(); // cancelled

    return QString::fromNSString(delegate.selectedUrl.path);
}

QString SaveFileSpecialDialog::getSaveFileName()
{
    // Default filename
    NSString *defaultName = @"weights.sqlite";

    // Create a temporary file to "export"
    NSString *tempPath = [NSTemporaryDirectory() stringByAppendingPathComponent:defaultName];
    NSURL *tempUrl = [NSURL fileURLWithPath:tempPath];

    // Create an empty file
    [@"" writeToURL:tempUrl atomically:YES encoding:NSUTF8StringEncoding error:nil];

    // Create picker in export mode
    UIDocumentPickerViewController *picker =
        [[UIDocumentPickerViewController alloc]
            initWithURLs:@[tempUrl]
            inMode:UIDocumentPickerModeExportToService];

    SaveFilePickerDelegate *delegate = [SaveFilePickerDelegate new];
    picker.delegate = delegate;

    // Present modally
    UIViewController *root = UIApplication.sharedApplication.keyWindow.rootViewController;
    [root presentViewController:picker animated:YES completion:nil];

    // Block until user finishes
    while (!delegate.finished) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:0.1]];
    }

    if (delegate.selectedUrl == nil)
        return QString(); // cancelled

    return QString::fromNSString(delegate.selectedUrl.path);
}

#else

// Non‑iOS fallback
QString SaveFileSpecialDialog::getSaveFileName()
{
    return QString();
}

#endif
