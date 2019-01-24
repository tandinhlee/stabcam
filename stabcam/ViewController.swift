//
//  ViewController.swift
//  stabcam
//
//  Created by Dinh Le on 1/12/19.
//  Copyright © 2019 Dinh Le. All rights reserved.
//

import UIKit
import Photos

class ViewController: UIViewController, OpenCVWrapperDelegate {
    
    @IBOutlet weak var videoView: UIImageView!
    @IBOutlet weak var originalView: UIImageView!
    var opencvWrapper: OpenCVWrapper!
    var isStart:Bool = false
    override func viewDidLoad() {
        super.viewDidLoad()
        opencvWrapper = OpenCVWrapper.init(withVideoParentView: videoView)
        // Do any additional setup after loading the view, typically from a nib.
    }
    @IBAction func startRecording(_ sender: Any) {
        opencvWrapper.actionStart(self);
        opencvWrapper.assign(self);
    }
    func didReceiveOriginalImage(_ originalImage: UIImage, processedImage processImage: UIImage) {
        DispatchQueue.main.async { () -> Void in
            self.originalView.image = originalImage
        }
    }
}
